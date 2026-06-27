#pragma once
#include <QObject>
#include <QTimer>
#include <QString>

class GameEngine : public QObject {
    Q_OBJECT // Обязательный макрос Qt для работы реактивности

    // Регистрируем свойства для QML (как стейт в React)
    Q_PROPERTY(int timeLeft READ timeLeft NOTIFY timeLeftChanged)
    Q_PROPERTY(bool isGameOver READ isGameOver NOTIFY isGameOverChanged)
    Q_PROPERTY(bool doorLocked READ doorLocked NOTIFY doorLockedChanged)
    Q_PROPERTY(QString gameStatus READ gameStatus NOTIFY gameStatusChanged)

    // Координаты игрока теперь хранятся в C++
    Q_PROPERTY(int playerX READ playerX NOTIFY playerPositionChanged)
    Q_PROPERTY(int playerY READ playerY NOTIFY playerPositionChanged)

    // Мосты для Стены
    Q_PROPERTY(int wallX READ wallX CONSTANT)
    Q_PROPERTY(int wallY READ wallY CONSTANT)
    Q_PROPERTY(int wallWidth READ wallWidth CONSTANT)
    Q_PROPERTY(int wallHeight READ wallHeight CONSTANT)

    // Мосты для Двери
    Q_PROPERTY(int doorX READ doorX CONSTANT)
    Q_PROPERTY(int doorY READ doorY CONSTANT)
    Q_PROPERTY(int doorWidth READ doorWidth CONSTANT)
    Q_PROPERTY(int doorHeight READ doorHeight CONSTANT)

    // Мосты для Зоны
    Q_PROPERTY(int safeZoneX READ safeZoneX CONSTANT)
    Q_PROPERTY(int safeZoneY READ safeZoneY CONSTANT)
    Q_PROPERTY(int safeZoneWidth READ safeZoneWidth CONSTANT)
    Q_PROPERTY(int safeZoneHeight READ safeZoneHeight CONSTANT)

public:
    explicit GameEngine(QObject *parent = nullptr);

    // Геттеры для чтения данных из QML
    int timeLeft() const { return m_timeLeft; }
    bool isGameOver() const { return m_isGameOver; }
    bool doorLocked() const { return m_doorLocked; }
    QString gameStatus() const { return m_gameStatus; }
    int playerX() const { return m_playerX; }
    int playerY() const { return m_playerY; }

    // Геттеры для Стены
    int wallX() const { return m_wallGeometry[0]; }
    int wallY() const { return m_wallGeometry[1]; }
    int wallWidth() const { return m_wallGeometry[2]; }
    int wallHeight() const { return m_wallGeometry[3]; }

    // Геттеры для Двери
    int doorX() const { return m_doorGeometry[0]; }
    int doorY() const { return m_doorGeometry[1]; }
    int doorWidth() const { return m_doorGeometry[2]; }
    int doorHeight() const { return m_doorGeometry[3]; }

    // Геттеры для Зоны
    int safeZoneX() const { return m_safeZoneGeometry[0]; }
    int safeZoneY() const { return m_safeZoneGeometry[1]; }
    int safeZoneWidth() const { return m_safeZoneGeometry[2]; }
    int safeZoneHeight() const { return m_safeZoneGeometry[3]; }

    // Этот макрос разрешает вызывать метод C++ прямо из QML-кода кнопок
    Q_INVOKABLE void handleKeyPress(const QString &key);

signals:
    // Сигналы-уведомления для авто-перерисовки UI
    void timeLeftChanged();
    void isGameOverChanged();
    void doorLockedChanged();
    void gameStatusChanged();
    void playerPositionChanged();

private slots:
    void onTimerTick(); // Метод для обработки тика секундного таймера

private:
    // Внутреннее состояние игры (State)
    int m_timeLeft = 20;
    bool m_isGameOver = false;
    bool m_doorLocked = true;
    QString m_gameStatus = "Доберись до ДВЕРИ и вернись в ЗОНУ!";

    int m_playerX = 392;
    int m_playerY = 717;

    int m_wallGeometry[4] = {200, 500, 400, 30};
    int m_doorGeometry[4] = {390, 200, 20, 60};
    int m_safeZoneGeometry[4] = {375, 700, 50, 50};

    // Константы размеров
    int m_playerStep = 20;
    const int m_mapSize = 800;   // Размер игрового поля
    const int m_playerSize = 16; // Размер кубика вора (ширина и высота)

    QTimer *m_timer; // Плюсовый таймер

    // Чистая C++ математика коллизий (AABB)
    bool checkCollision(int nx, int ny, int ox, int oy, int ow, int oh);
};
