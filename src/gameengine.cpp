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
GameEngine::GameEngine(int level, QObject *parent) : QObject(parent) {
    m_map = new GameMap(level, this);
    m_player = new Character(365, 667, this);
    m_player->setIsInCar(true);
    // m_assistant = new Character(7_grid, 14_grid, this);
    m_playerStep = m_map->gridSize();

    m_car = new CarObject(-20, 14_gridSize, 2_gridSize, 1_gridSize, this);
    m_gameObjects.push_back(m_car);

    if (level == 1) {
        auto* door = new DoorObject(7_gridSize, 4_gridSize,  1_gridSize, static_cast<int>(1_gridSize * 0.4), this);
        auto* safe1 = new SafeObject(5_gridSize, 6_gridSize,  1_gridSize, 1_gridSize, this);
        auto* safe2 = new SafeObject(11_gridSize, 10_gridSize,  1_gridSize, 1_gridSize, this);

        m_gameObjects.push_back(door);
        m_gameObjects.push_back(safe1);
        m_gameObjects.push_back(safe2);
    } else {
        // Внутренняя ДВЕРЬ №1 (Вход в левую комнату, X = 5)
        auto* door1 = new DoorObject(5_gridSize, 7_gridSize, 1_gridSize, 1_gridSize, this);

        // Внутренняя ДВЕРЬ №2 (Вход в правую комнату, X = 10)
        auto* door2 = new DoorObject(10_gridSize, 7_gridSize, 1_gridSize, 1_gridSize, this);

        // ДОБАВИЛИ: ПАРАДНАЯ ДВЕРЬ №3 (Вход с улицы в банк, X = 7, Y = 12)
        auto* door3 = new DoorObject(7_gridSize, 12_gridSize, 2_gridSize, 1_gridSize, this); // Сделаем её шириной в 2 клетки (2_gridSize)!

        // СЕЙФ №1 (Левое крыло)
        auto* safe1 = new SafeObject(4_gridSize, 4_gridSize, 1_gridSize, 1_gridSize, this);

        // СЕЙФ №2 (Правое крыло)
        auto* safe2 = new SafeObject(12_gridSize, 4_gridSize, 1_gridSize, 1_gridSize, this);

        // Пушим все три двери и сейфы в вектор объектов
        m_gameObjects.push_back(m_car);
        m_gameObjects.push_back(door1);
        m_gameObjects.push_back(door2);
        m_gameObjects.push_back(door3); // Не забываем запушить третью дверь!
        m_gameObjects.push_back(safe1);
        m_gameObjects.push_back(safe2);
    }



    // TIMER START
    m_timeManager = new TimeManager(this);

    // Связываем сигнал окончания времени из TimeManager со слотом конца игры в движке
    connect(m_timeManager, &TimeManager::timeUp, this, &GameEngine::handleTimeUp);
    connect(m_car, &CarObject::arrivalFinished, this, [this](){
        m_isMoving = false;
        m_player->setIsInCar(false);
    });

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
        int targetX = mapSize() / 2 - m_map->gridSize();
        m_car->arrive(targetX);
    });
}

int GameEngine::mapSize() const {
    return m_map->mapSize();
}

QVariantList GameEngine::gameObjects() const {
    QVariantList list;
    for (auto* obj : m_gameObjects) {
        list.append(QVariant::fromValue(obj));
    }
    return list;
}

// СЕКУНДНЫЙ ТИК ТАЙМЕРА (асинхронно уменьшает время)
void GameEngine::handleTimeUp() {
    if (m_isGameOver) return; // Если игра окончена, ничего не делаем

    m_isGameOver = true;
    emit isGameOverChanged(); // Сигнализируем QML, что управление пора заблочить

    QRect playerRect(m_player->x(), m_player->y(), m_playerSize, m_playerSize);
    bool inCar = playerRect.intersects(m_car->rect());
    m_player->setIsInCar(inCar);

    m_car->escape(mapSize());

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
    if (m_isGameOver || m_isMoving || m_player->isBusy()) return;

    int nextX = m_player->x();
    int nextY = m_player->y();

    // Сравниваем строки на чистом C++
    if (key == "Left")  { nextX -= m_playerStep; }
    if (key == "Right") { nextX += m_playerStep; }
    if (key == "Up")    { nextY -= m_playerStep; }
    if (key == "Down")  { nextY += m_playerStep; }

    QRect nextPlayerRect(nextX, nextY, m_playerSize, m_playerSize);

    if (m_player->isInCar()) {
        m_player->setIsInCar(false);
    }

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
}

void GameEngine::onMoveFinished() {
    m_isMoving = false; // Время анимации вышло, открываем замок для следующего шага!
}
