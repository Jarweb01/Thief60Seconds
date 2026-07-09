#include "gameengine.h"
#include "core/GameMap.h"
#include "core/TimeManager.h"
#include "entities/Character.h"
#include "objects/InteractableObject.h"
#include "objects/DoorObject.h"
#include "objects/SafeObject.h"
#include "objects/CarObject.h"
#include <algorithm> // Нужен для функций std::max и std::min (ограничители движения)
#include <QVariantMap>
#include <QDebug>

// Оператор кастомного литерала. Теперь суффикс _grid будет автоматически умножать число на 50
constexpr int operator"" _gridSize(unsigned long long int cells) {
    return cells * 50;
}

// 1. КОНСТРУКТОР: вызывается в момент создания игры в памяти
GameEngine::GameEngine(QObject *parent) : QObject(parent) {
    m_map = new GameMap(this);
    m_player = new Character(365, 667, this);
    m_player->setIsInCar(true);
    // m_assistant = new Character(7_grid, 14_grid, this);
    m_playerStep = m_map->gridSize();

    auto* car = new CarObject(7_gridSize, 14_gridSize, 2_gridSize, 1_gridSize, this);
    auto* door = new DoorObject(7_gridSize, 4_gridSize,  1_gridSize, static_cast<int>(1_gridSize * 0.4), this);
    auto* safe = new SafeObject(5_gridSize, 6_gridSize,  1_gridSize, 1_gridSize, this);

    m_carRef = car;
    m_doorRef = door;
    m_safeRef = safe;

    m_gameObjects.push_back(car);
    m_gameObjects.push_back(door);
    m_gameObjects.push_back(safe);

    // TIMER START
    m_timeManager = new TimeManager(this);

    // Связываем сигнал окончания времени из TimeManager со слотом конца игры в движке
    connect(m_timeManager, &TimeManager::timeUp, this, &GameEngine::handleTimeUp);

    // Настраиваем таймер кулдауна движения
    m_moveCooldownTimer = new QTimer(this);
    m_moveCooldownTimer->setSingleShot(true); // Таймер сработает ровно один раз за шаг
    m_moveCooldownTimer->setInterval(m_moveDuration);    // Блокировка на n миллисекунд (длительность шага)
    connect(m_moveCooldownTimer, &QTimer::timeout, this, &GameEngine::onMoveFinished);
    // TIMER END
}

GameEngine::~GameEngine() {}

void GameEngine::startLevel() {
    if (m_timeManager) {
        m_timeManager->start();
    }

    QTimer::singleShot(100, this, [this]() {
        m_carX = mapSize() / 2 - m_map->gridSize();
        emit carXChanged(); // Кидаем сигнал: координата carX изменилась
    });
}

int GameEngine::mapSize() const {
    return m_map->mapSize();
}

// СЕКУНДНЫЙ ТИК ТАЙМЕРА (асинхронно уменьшает время)
void GameEngine::handleTimeUp() {
    if (m_isGameOver) return; // Если игра окончена, ничего не делаем

    m_isGameOver = true;
    emit isGameOverChanged(); // Сигнализируем QML, что управление пора заблочить

    m_carState = 2;
    m_carX = mapSize();
    emit carStateChanged();
    emit carXChanged();

    QRect playerRect(m_player->x(), m_player->y(), m_playerSize, m_playerSize);
    bool inCar = playerRect.intersects(m_carRef->rect());
    m_player->setIsInCar(inCar);

    // Проверяем, вернулся ли игрок к Машине
    if (m_player->isInCar()) {
        m_gameStatus = "ВЫ ВЫИГРАЛИ!";
    } else {
        m_gameStatus = "ИГРА ОКОНЧЕНА! Машина уехала, вы остались!";
    }

    emit gameStatusChanged(); // Обновляем текст статуса в UI
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

    QRect nextPlayerRect(nextX, nextY, m_playerSize, m_playerSize);

    for(auto* object : m_gameObjects) {
        if (nextPlayerRect.intersects(object->rect())) {
            bool isBlocking = object->onCollision(m_player);

            if (isBlocking) {
                return;
            }
        }
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

    QRect currentPlayerRect(m_player->x(), m_player->y(), m_playerSize, m_playerSize);

    bool isInCar = currentPlayerRect.intersects(m_carRef->rect());
    m_player->setIsInCar(isInCar);

    m_doorLocked = m_doorRef->isStateActive();
    emit doorLockedChanged();
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
