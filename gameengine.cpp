#include "gameengine.h"
#include <algorithm> // Нужен для функций std::max и std::min (ограничители движения)
#include <random>
#include <QVariantMap>

// 1. КОНСТРУКТОР: вызывается в момент создания игры в памяти
GameEngine::GameEngine(QObject *parent) : QObject(parent) {
    // 1. Инициализируем генератор случайных чисел
    std::random_device rd;  // Получаем аппаратное случайное число для затравки
    std::mt19937 gen(rd()); // Инициализируем стандартный генератор Вихрь Мерсенна

    // 2. Задаем диапазон распределения
    std::uniform_int_distribution<> distr(50, 60);

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

}

// 2. СЕКУНДНЫЙ ТИК ТАЙМЕРА (асинхронно уменьшает время)
void GameEngine::onTimerTick() {
    if (m_isGameOver) return; // Если игра окончена, ничего не делаем

    m_timeLeft--;
    emit timeLeftChanged(); // Стреляем сигналом: QML тут же обновит цифру на экране

    if (m_timeLeft == 0) {
        m_isGameOver = true;
        emit isGameOverChanged(); // Сигнализируем QML, что управление пора заблочить
        m_timer->stop();

        // Проверяем, вернулся ли игрок в ЗОНУ
        if (checkCollision(m_playerX, m_playerY, m_safeZoneGeometry[0], m_safeZoneGeometry[1], m_safeZoneGeometry[2], m_safeZoneGeometry[3])) {
            m_gameStatus = "ВЫ ВЫИГРАЛИ! Успели в зону!";
        } else {
            m_gameStatus = "ИГРА ОКОНЧЕНА! Не успели в зону!";
        }
        emit gameStatusChanged(); // Обновляем текст статуса в UI
    }
}

// 3. ОБРАБОТКА НАЖАТИЯ КНОПОК (Вызывается из JS-кода в QML)
void GameEngine::handleKeyPress(const QString &key) {
    if (m_isGameOver || m_isMoving) return;

    int nextX = m_playerX;
    int nextY = m_playerY;

    // Сравниваем строки на чистом C++
    if (key == "Left")  { nextX -= m_playerStep; }
    if (key == "Right") { nextX += m_playerStep; }
    if (key == "Up")    { nextY -= m_playerStep; }
    if (key == "Down")  { nextY += m_playerStep; }

    // 1. ПРОВЕРКА СЕЙФА (Действует как стена + триггер взлома)
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
    bool hitAnyWall = false;
    // Циклом проверяем столкновение с каждой из 4 стен комнаты
    for (size_t i = 0; i < m_wallsLayout.size(); ++i) {
        if (checkCollision(nextX, nextY, m_wallsLayout[i].x, m_wallsLayout[i].y, m_wallsLayout[i].w, m_wallsLayout[i].h)) {
            hitAnyWall = true;
            break;
        }
    }

    // Если впереди чисто — двигаемся
    if (!hitAnyWall) {
        m_playerX = std::max(0, std::min(nextX, m_mapSize - m_playerSize));
        m_playerY = std::max(0, std::min(nextY, m_mapSize - m_playerSize));

        // Если координаты реально изменились, включаем блокировку
        m_isMoving = true;             // Закрываем замок для кнопок
        m_moveCooldownTimer->start(); // Запускаем отсчет n миллисекунд

        emit playerPositionChanged();
    }

    // Проверяем касание ДВЕРИ
    if (checkCollision(m_playerX, m_playerY, m_doorGeometry[0], m_doorGeometry[1], m_doorGeometry[2], m_doorGeometry[3])) {
        m_doorLocked = false;

        emit doorLockedChanged(); // Дверь в QML мгновенно зеленеет
    }
}

// 4. МАТЕМАТИКА КОЛЛИЗИЙ (AABB столкновение прямоугольников)
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

QVariantList GameEngine::walls() const {
    QVariantList list;
    for (size_t i = 0; i < m_wallsLayout.size(); ++i) {
        QVariantMap map;
        map["x"] = m_wallsLayout[i].x;
        map["y"] = m_wallsLayout[i].y;
        map["w"] = m_wallsLayout[i].w;
        map["h"] = m_wallsLayout[i].h;
        list.append(map);
    }
    return list;
}
