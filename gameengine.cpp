#include "gameengine.h"
#include <algorithm> // Нужен для функций std::max и std::min (ограничители движения)

// 1. КОНСТРУКТОР: вызывается в момент создания игры в памяти
GameEngine::GameEngine(QObject *parent) : QObject(parent) {
    m_timer = new QTimer(this); // Выделяем память под плюсовый таймер
    m_timer->setInterval(1000); // Задаем интервал тика в 1000 миллисекунд (1 секунда)

    // Связываем сигнал таймера (timeout) со слотом (нашей функцией onTimerTick)
    connect(m_timer, &QTimer::timeout, this, &GameEngine::onTimerTick);

    m_timer->start(); // Запускаем обратный отсчет таймера
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

        // Проверяем, вернулся ли игрок в ЗОНУ (координаты safeZone: x=175, y=330, w=50, h=50)
        if (checkCollision(m_playerX, m_playerY, 175, 330, 50, 50)) {
            m_gameStatus = "ВЫ ВЫИГРАЛИ! Успели в зону!";
        } else {
            m_gameStatus = "ИГРА ОКОНЧЕНА! Не успели в зону!";
        }
        emit gameStatusChanged(); // Обновляем текст статуса в UI
    }
}

// 3. ОБРАБОТКА НАЖАТИЯ КНОПОК (Вызывается из JS-кода в QML)
void GameEngine::handleKeyPress(const QString &key) {
    if (m_isGameOver) return; // Если игра завершена, кнопки не реагируют

    int nextX = m_playerX;
    int nextY = m_playerY;
    int step = 20; // Шаг вора в пикселях

    // Сравниваем строки на чистом C++
    if (key == "Left")  { nextX -= step; }
    if (key == "Right") { nextX += step; }
    if (key == "Up")    { nextY -= step; }
    if (key == "Down")  { nextY += step; }

    // Проверяем столкновение со СТЕНОЙ (координаты wall: x=150, y=50, w=50, h=200)
    if (!checkCollision(nextX, nextY, 150, 50, 50, 200)) {
        // Если впереди нет стены, ограничиваем движение в рамках карты 800х800 (Clamp)
        // Игрок упрется в края экрана, но не вылетит за них
        m_playerX = std::max(0, std::min(nextX, 800 - 16));
        m_playerY = std::max(0, std::min(nextY, 800 - 16));

        emit playerPositionChanged(); // Выстреливаем сигнал, и вор плавно плывет в UI
    }

    // Проверяем касание ДВЕРИ (координаты door: x=300, y=150, w=20, h=60)
    if (checkCollision(m_playerX, m_playerY, 300, 150, 20, 60)) {
        m_doorLocked = false;
        m_gameStatus = "Дверь взята! Срочно беги назад в ЗОНУ!";

        emit doorLockedChanged(); // Дверь в QML мгновенно зеленеет
        emit gameStatusChanged(); // Текст хедера обновляется
    }
}

// 4. МАТЕМАТИКА КОЛЛИЗИЙ (AABB столкновение прямоугольников)
bool GameEngine::checkCollision(int nx, int ny, int ox, int oy, int ow, int oh) {
    // Чистая геометрия: игрок имеет размеры 16х16 пикселей
    return (nx < ox + ow &&
            nx + 16 > ox &&
            ny < oy + oh &&
            ny + 16 > oy);
}
