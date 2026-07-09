#pragma once
#include <QObject>
#include <QTimer>
#include <QString>
#include <QVariant>
#include <vector>
#include "objects/InteractableObject.h"

class Character;
class GameMap;
class InteractableObject;
class TimeManager;

class GameEngine : public QObject {
    Q_OBJECT // Обязательный макрос Qt для работы реактивности

    // Регистрируем свойства для QML
    Q_PROPERTY(TimeManager* timeManager READ timeManager CONSTANT)
    Q_PROPERTY(bool isGameOver READ isGameOver NOTIFY isGameOverChanged)
    Q_PROPERTY(QString gameStatus READ gameStatus NOTIFY gameStatusChanged)
    Q_PROPERTY(int moveDuration READ moveDuration CONSTANT)

    // Координаты игрока
    Q_PROPERTY(Character* player READ player CONSTANT)
    // Q_PROPERTY(Character* assistant READ assistant CONSTANT)

    // Мосты для Стен
    Q_PROPERTY(GameMap* map READ map CONSTANT)
    Q_PROPERTY(QVariantList gameObjects READ gameObjects CONSTANT)

    Q_PROPERTY(int carState READ carState NOTIFY carStateChanged)
    Q_PROPERTY(int carX READ carX NOTIFY carXChanged)

public:
    explicit GameEngine(QObject *parent = nullptr);
    ~GameEngine() override;

    Character* player() const { return m_player; }
    // Character* assistant() const { return m_assistant; }

    GameMap* map() const { return m_map; }

    // Геттеры для чтения данных из QML
    TimeManager* timeManager() const { return m_timeManager; }
    QVariantList gameObjects() const;

    bool isGameOver() const { return m_isGameOver; }
    QString gameStatus() const { return m_gameStatus; }
    int moveDuration() const { return m_moveDuration; }
    int mapSize() const;

    // Геттеры для Машины
    int carState() const { return m_carState; }
    int carX() const { return m_carX; }

    // Этот макрос разрешает вызывать метод C++ прямо из QML-кода кнопок
    Q_INVOKABLE void handleKeyPress(const QString &key);
    Q_INVOKABLE void startLevel();

signals:
    // Сигналы-уведомления для авто-перерисовки UI
    void isGameOverChanged();
    void gameStatusChanged();
    void safeLootedChanged();
    void carStateChanged();
    void carXChanged();

public slots:
    void onCarArrived();
    void handleTimeUp();

private slots:
    void onMoveFinished();

private:
    // Внутреннее состояние игры (State)
    bool m_isGameOver = false;
    QString m_gameStatus = "Доберись до СЕЙФА и вернись к МАШИНЕ!";

    // Car
    int m_carState = 0; // 0 - приезд, 1 - игра, 2 - побег
    int m_carX = -20;

    // InteractableObjects
    std::vector<InteractableObject*> m_gameObjects;

    // Константы размеров
    // Player
    Character* m_player = nullptr;
    // Character* m_assistant = nullptr;
    GameMap* m_map = nullptr;

    int m_playerStep;
    const int m_moveDuration = 400; // Скорость шага в миллисекундах
    const int m_playerSize = 16; // Размер кубика вора (ширина и высота)

    bool m_isMoving = true;

    // timer
    QTimer *m_moveCooldownTimer; // Таймер блокировки кнопок на время анимации
    TimeManager* m_timeManager = nullptr;
};
