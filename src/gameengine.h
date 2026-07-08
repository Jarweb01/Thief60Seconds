#pragma once
#include <QObject>
#include <QTimer>
#include <QString>
#include <QVariant>
// #include <vector>

// Оператор кастомного литерала. Теперь суффикс _grid будет автоматически умножать число на 50
constexpr int operator"" _gridSize(unsigned long long int cells) {
    return cells * 50;
}

class Character;
class GameMap;

class GameEngine : public QObject {
    Q_OBJECT // Обязательный макрос Qt для работы реактивности

    // Регистрируем свойства для QML (как стейт в React)
    Q_PROPERTY(int timeLeft READ timeLeft NOTIFY timeLeftChanged)
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
    Q_PROPERTY(int doorX READ doorX CONSTANT)
    Q_PROPERTY(int doorY READ doorY CONSTANT)
    Q_PROPERTY(int doorWidth READ doorWidth CONSTANT)
    Q_PROPERTY(int doorHeight READ doorHeight CONSTANT)

    // Мосты для Машины
    Q_PROPERTY(int safeZoneX READ safeZoneX CONSTANT)
    Q_PROPERTY(int safeZoneY READ safeZoneY CONSTANT)
    Q_PROPERTY(int safeZoneWidth READ safeZoneWidth CONSTANT)
    Q_PROPERTY(int safeZoneHeight READ safeZoneHeight CONSTANT)
    Q_PROPERTY(int carState READ carState NOTIFY carStateChanged)
    Q_PROPERTY(int carX READ carX NOTIFY carXChanged)

    // Мосты для Сейфа
    Q_PROPERTY(int safeX READ safeX CONSTANT)
    Q_PROPERTY(int safeY READ safeY CONSTANT)
    Q_PROPERTY(int safeWidth READ safeWidth CONSTANT)
    Q_PROPERTY(int safeHeight READ safeHeight CONSTANT)
    Q_PROPERTY(bool safeLooted READ safeLooted NOTIFY safeLootedChanged)


public:
    explicit GameEngine(QObject *parent = nullptr);
    ~GameEngine() override;

    Character* player() const { return m_player; }
    // Character* assistant() const { return m_assistant; }

    GameMap* map() const { return m_map; }

    // Геттеры для чтения данных из QML
    int timeLeft() const { return m_timeLeft; }
    bool isGameOver() const { return m_isGameOver; }
    bool doorLocked() const { return m_doorLocked; }
    QString gameStatus() const { return m_gameStatus; }
    int moveDuration() const { return m_moveDuration; }
    int mapSize() const;

    // Геттеры для Двери
    int doorX() const { return m_doorGeometry[0]; }
    int doorY() const { return m_doorGeometry[1]; }
    int doorWidth() const { return m_doorGeometry[2]; }
    int doorHeight() const { return m_doorGeometry[3]; }

    // Геттеры для Машины
    int safeZoneX() const { return m_safeZoneGeometry[0]; }
    int safeZoneY() const { return m_safeZoneGeometry[1]; }
    int safeZoneWidth() const { return m_safeZoneGeometry[2]; }
    int safeZoneHeight() const { return m_safeZoneGeometry[3]; }
    int carState() const { return m_carState; }
    int carX() const { return m_carX; }


    // Геттеры для Сейфа
    int safeX() const      { return m_safeGeometry[0]; }
    int safeY() const      { return m_safeGeometry[1]; }
    int safeWidth() const  { return m_safeGeometry[2]; }
    int safeHeight() const { return m_safeGeometry[3]; }
    bool safeLooted() const { return m_safeLooted; }


    // Этот макрос разрешает вызывать метод C++ прямо из QML-кода кнопок
    Q_INVOKABLE void handleKeyPress(const QString &key);

signals:
    // Сигналы-уведомления для авто-перерисовки UI
    void timeLeftChanged();
    void isGameOverChanged();
    void doorLockedChanged();
    void gameStatusChanged();
    void safeLootedChanged();
    void carStateChanged();
    void carXChanged();

public slots:
    void onCarArrived();

private slots:
    void onTimerTick(); // Метод для обработки тика секундного таймера
    void onMoveFinished();

private:
    // Внутреннее состояние игры (State)
    int m_timeLeft; // Инициализируется рандомом в .cpp
    bool m_isGameOver = false;
    bool m_doorLocked = true;
    QString m_gameStatus = "Доберись до СЕЙФА и вернись к МАШИНЕ!";

    // Car
    int m_safeZoneGeometry[4] = {7_gridSize, 14_gridSize, 2_gridSize, 1_gridSize};
    int m_carState = 0; // 0 - приезд, 1 - игра, 2 - побег
    int m_carX = -20;

    int m_doorGeometry[4] = {7_gridSize, 4_gridSize, 1_gridSize, static_cast<int>(1_gridSize * 0.4)};

    // Safe
    bool m_safeLooted = false;
    int m_safeGeometry[4] = {5_gridSize, 6_gridSize,  1_gridSize, 1_gridSize};


    // Константы размеров
    // Player
    Character* m_player = nullptr;
    // Character* m_assistant = nullptr;
    GameMap* m_map = nullptr;

    int m_playerStep;
    const int m_moveDuration = 400; // Скорость шага в миллисекундах

    const int m_playerSize = 16; // Размер кубика вора (ширина и высота)

    bool m_isMoving = true;

    // timers
    QTimer *m_moveCooldownTimer; // Таймер блокировки кнопок на время анимации
    QTimer *m_timer; // Плюсовый таймер

    // Чистая C++ математика коллизий (AABB)
    bool checkCollision(int nx, int ny, int ox, int oy, int ow, int oh);
};
