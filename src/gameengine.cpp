#include "gameengine.h"
#include "GameMap.h"
#include "Character.h"
#include <algorithm> // Нужен для функций std::max и std::min (ограничители движения)
#include <random>
#include <QVariantMap>
#include <QDebug>

// 1. КОНСТРУКТОР: вызывается в момент создания игры в памяти
GameEngine::GameEngine(QObject *parent) : QObject(parent) {
    m_map = new GameMap(this);

    m_player = new Character(365, 667, this);
    m_player->setIsInCar(true);
    // m_assistant = new Character(7_grid, 14_grid, this);

    m_playerStep = m_map->gridSize();

    // 1. Инициализируем генератор случайных чисел
    std::random_device rd;  // Получаем аппаратное случайное число для затравки
    std::mt19937 gen(rd()); // Инициализируем стандартный генератор Вихрь Мерсенна

    // 2. Задаем диапазон распределения
    std::uniform_int_distribution<> distr(250, 260);

    // 3. Присваиваем случайное число нашей переменной времени
    m_timeLeft = distr(gen);

    m_timer = new QTimer(this); // Выделяем память под плюсовый таймер
    m_timer->setInterval(1000); // Задаем интервал тика в 1000 миллисекунд (1 секунда)

    // Связываем сигнал таймера (timeout) со слотом (нашей функцией onTimerTick)
    connect(m_timer, &QTimer::timeout, this, &GameEngine::onTimerTick);

    m_timer->start(); // Запускаем обратный отсчет таймера

    // Настраиваем таймер кулдауна движения
    m_moveCooldownTimer = new QTimer(this);
    m_moveCooldownTimer->setSingleShot(true); // Таймер сработает ровно один раз за шаг
    m_moveCooldownTimer->setInterval(m_moveDuration);    // Блокировка на n миллисекунд (длительность шага)
    connect(m_moveCooldownTimer, &QTimer::timeout, this, &GameEngine::onMoveFinished);

    QTimer::singleShot(100, this, [this]() {
        m_carX = mapSize() / 2 - m_map->gridSize();
        emit carXChanged(); // Кидаем сигнал: координата carX изменилась
    });
}

GameEngine::~GameEngine() {}

int GameEngine::mapSize() const {
    return m_map->mapSize();
}

// СЕКУНДНЫЙ ТИК ТАЙМЕРА (асинхронно уменьшает время)
void GameEngine::onTimerTick() {
    if (m_isGameOver) return; // Если игра окончена, ничего не делаем

    m_timeLeft--;
    emit timeLeftChanged(); // Стреляем сигналом: QML тут же обновит цифру на экране

    if (m_timeLeft == 0) {
        m_isGameOver = true;
        emit isGameOverChanged(); // Сигнализируем QML, что управление пора заблочить
        m_timer->stop();

        m_carState = 2;
        m_carX = mapSize();

        emit carStateChanged();
        emit carXChanged();

        bool inCar = checkCollision(m_player->x(), m_player->y(), m_safeZoneGeometry[0], m_safeZoneGeometry[1], m_safeZoneGeometry[2], m_safeZoneGeometry[3]);
        m_player->setIsInCar(inCar);

        // Проверяем, вернулся ли игрок к Машине
        if (m_player->isInCar()) {
            m_gameStatus = "ВЫ ВЫИГРАЛИ!";
        } else {
            m_gameStatus = "ИГРА ОКОНЧЕНА! Машина уехала, вы остались!";
        }

        emit gameStatusChanged(); // Обновляем текст статуса в UI
    }
}

// 3. ОБРАБОТКА НАЖАТИЯ КНОПОК (Вызывается из JS-кода в QML)
void GameEngine::handleKeyPress(const QString &key) {
    if (m_isGameOver || m_isMoving) return;

    int nextX = m_player->x();
    int nextY = m_player->y();

    // Сравниваем строки на чистом C++
    if (key == "Left")  { nextX -= m_playerStep; }
    if (key == "Right") { nextX += m_playerStep; }
    if (key == "Up")    { nextY -= m_playerStep; }
    if (key == "Down")  { nextY += m_playerStep; }

    // ПРОВЕРКА СЕЙФА (Действует как стена + триггер взлома)
    // Проверяем, натыкается ли СЛЕДУЮЩИЙ шаг вора на координаты сейфа
    if(checkCollision(nextX, nextY, m_safeGeometry[0], m_safeGeometry[1], m_safeGeometry[2], m_safeGeometry[3])) {

        // Если сейф ещё не был взломан — взламываем!
        if(!m_safeLooted){
            m_safeLooted = true;

            emit safeLootedChanged();
        }

        return;
    }

    // Проверяем столкновение со СТЕНОЙ
    if (m_map->isWallAt(nextX, nextY, m_playerSize)) {
        return;
    }

    // Если впереди чисто — двигаемся
    int clampedX = std::max(0, std::min(nextX, mapSize() - m_playerSize));
    int clampedY = std::max(0, std::min(nextY, mapSize() - m_playerSize));

    m_player->setX(clampedX);
    m_player->setY(clampedY);

    m_isMoving = true;             // Закрываем замок для кнопок
    m_moveCooldownTimer->start(); // Запускаем отсчет n миллисекунд

    bool isInCar = checkCollision(m_player->x(), m_player->y(), m_safeZoneGeometry[0], m_safeZoneGeometry[1], m_safeZoneGeometry[2], m_safeZoneGeometry[3]);
    m_player->setIsInCar(isInCar);

    // Проверяем касание ДВЕРИ
    if (checkCollision(m_player->x(), m_player->y(), m_doorGeometry[0], m_doorGeometry[1], m_doorGeometry[2], m_doorGeometry[3])) {
        m_doorLocked = false;
        emit doorLockedChanged(); // Дверь в QML мгновенно зеленеет
    }
}

// МАТЕМАТИКА КОЛЛИЗИЙ (AABB столкновение прямоугольников)
bool GameEngine::checkCollision(int nx, int ny, int ox, int oy, int ow, int oh) {
    // Чистая геометрия
    return (nx < ox + ow &&
            nx + m_playerSize > ox &&
            ny < oy + oh &&
            ny + m_playerSize > oy);
}

void GameEngine::onMoveFinished() {
    m_isMoving = false; // Время анимации вышло, открываем замок для следующего шага!
}

void GameEngine::onCarArrived() {
    qDebug() << "C++: 2 секунды истекли! Переключаем carState в 1!";
    m_carState = 1; // Переводим машину в режим ожидания (стоим)
    m_isMoving = false; // Разрешаем игроку ходить

    m_player->setIsInCar(false);
    emit carStateChanged();
}
