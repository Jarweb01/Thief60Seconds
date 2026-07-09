#pragma once
#include <QObject>
#include <QTimer>
#include <QString>
#include <QVariant>

class Character;
class GameMap;
class InteractableObject;
class TimeManager;

class GameEngine : public QObject {
    Q_OBJECT // Обязательный макрос Qt для работы реактивности

    // Регистрируем свойства для QML
    Q_PROPERTY(TimeManager* timeManager READ timeManager CONSTANT)
    Q_PROPERTY(bool isGameOver READ isGameOver NOTIFY isGameOverChanged)
    Q_PROPERTY(bool doorLocked READ doorLocked NOTIFY doorLockedChanged)
    Q_PROPERTY(QString gameStatus READ gameStatus NOTIFY gameStatusChanged)
    Q_PROPERTY(int moveDuration READ moveDuration CONSTANT)

    // Координаты игрока
    Q_PROPERTY(Character* player READ player CONSTANT)
    // Q_PROPERTY(Character* assistant READ assistant CONSTANT)

    // Мосты для Стен
    Q_PROPERTY(GameMap* map READ map CONSTANT)

    // Мосты для Двери
    Q_PROPERTY(InteractableObject* door READ door CONSTANT)

    // Мосты для Машины
    Q_PROPERTY(InteractableObject* car READ car CONSTANT)

    Q_PROPERTY(int carState READ carState NOTIFY carStateChanged)
    Q_PROPERTY(int carX READ carX NOTIFY carXChanged)

    // Мосты для Сейфа
    Q_PROPERTY(InteractableObject* safe READ safe CONSTANT)

public:
    explicit GameEngine(QObject *parent = nullptr);
    ~GameEngine() override;

    Character* player() const { return m_player; }
    // Character* assistant() const { return m_assistant; }

    GameMap* map() const { return m_map; }

    // Геттеры для чтения данных из QML
    TimeManager* timeManager() const { return m_timeManager; }
    bool isGameOver() const { return m_isGameOver; }
    bool doorLocked() const { return m_doorLocked; }
    QString gameStatus() const { return m_gameStatus; }
    int moveDuration() const { return m_moveDuration; }
    int mapSize() const;

    // Геттеры для Двери
    InteractableObject* door() const { return m_door; }

    // Геттеры для Машины
    InteractableObject* car() const { return m_car; }
    int carState() const { return m_carState; }
    int carX() const { return m_carX; }

    // Геттеры для Сейфа
    InteractableObject* safe() const { return m_safe; }

    // Этот макрос разрешает вызывать метод C++ прямо из QML-кода кнопок
    Q_INVOKABLE void handleKeyPress(const QString &key);
    Q_INVOKABLE void startLevel();

signals:
    // Сигналы-уведомления для авто-перерисовки UI
    void isGameOverChanged();
    void doorLockedChanged();
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
    bool m_doorLocked = true;
    QString m_gameStatus = "Доберись до СЕЙФА и вернись к МАШИНЕ!";

    // Car
    InteractableObject* m_car = nullptr;
    int m_carState = 0; // 0 - приезд, 1 - игра, 2 - побег
    int m_carX = -20;

    // Door
    InteractableObject* m_door = nullptr;

    // Safe
    InteractableObject* m_safe = nullptr;


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
