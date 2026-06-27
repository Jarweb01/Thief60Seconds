#pragma once
#include <QObject>
#include <QTimer>
#include <QString>
#include <QVariant>
#include <vector>

// Оператор кастомного литерала. Теперь суффикс _grid будет автоматически умножать число на 50
constexpr int operator"" _grid(unsigned long long int cells) {
    return cells * 50;
}

struct WallData {
    int x;
    int y;
    int w;
    int h;
};


class GameEngine : public QObject {
    Q_OBJECT // Обязательный макрос Qt для работы реактивности

    // Регистрируем свойства для QML (как стейт в React)
    Q_PROPERTY(int timeLeft READ timeLeft NOTIFY timeLeftChanged)
    Q_PROPERTY(bool isGameOver READ isGameOver NOTIFY isGameOverChanged)
    Q_PROPERTY(bool doorLocked READ doorLocked NOTIFY doorLockedChanged)
    Q_PROPERTY(QString gameStatus READ gameStatus NOTIFY gameStatusChanged)
    Q_PROPERTY(int moveDuration READ moveDuration CONSTANT)
    Q_PROPERTY(int gridSize READ gridSize CONSTANT)

    // Координаты игрока теперь хранятся в C++
    Q_PROPERTY(int playerX READ playerX NOTIFY playerPositionChanged)
    Q_PROPERTY(int playerY READ playerY NOTIFY playerPositionChanged)

    // Мосты для Стен
    Q_PROPERTY(QVariantList walls READ walls CONSTANT)

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

    // Мосты для Сейфа
    Q_PROPERTY(int safeX READ safeX CONSTANT)
    Q_PROPERTY(int safeY READ safeY CONSTANT)
    Q_PROPERTY(int safeWidth READ safeWidth CONSTANT)
    Q_PROPERTY(int safeHeight READ safeHeight CONSTANT)


public:
    explicit GameEngine(QObject *parent = nullptr);

    // Геттеры для чтения данных из QML
    int timeLeft() const { return m_timeLeft; }
    bool isGameOver() const { return m_isGameOver; }
    bool doorLocked() const { return m_doorLocked; }
    QString gameStatus() const { return m_gameStatus; }
    int playerX() const { return m_playerX; }
    int playerY() const { return m_playerY; }
    int moveDuration() const { return m_moveDuration; }
    int gridSize() const { return m_gridSize; }

    // Геттеры для Стены
    QVariantList walls() const;

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

    // Геттеры для Сейфа
    int safeX() const      { return m_safeGeometry[0]; }
    int safeY() const      { return m_safeGeometry[1]; }
    int safeWidth() const  { return m_safeGeometry[2]; }
    int safeHeight() const { return m_safeGeometry[3]; }


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
    void onMoveFinished();

private:
    // Внутреннее состояние игры (State)
    int m_timeLeft; // Инициализируется рандомом в .cpp
    bool m_isGameOver = false;
    bool m_doorLocked = true;
    QString m_gameStatus = "Доберись до ДВЕРИ и вернись в ЗОНУ!";
    const int m_gridSize = 50;

    int m_playerX = 365;
    int m_playerY = 717;

    // std::vector позволяет добавлять сколько угодно стен, размер вычисляется динамически!
    std::vector<WallData> m_wallsLayout = {
        // Внешние стены здания (комната)
        { 4_grid, 4_grid,  8_grid, 30 },     // Верх
        { 4_grid, 4_grid,  30,     7_grid }, // Лево
        { 12_grid, 4_grid, 30,     7_grid }, // Право
        { 4_grid, 11_grid, 8_grid, 30 },     // Низ

        // ВНУТРЕННИЕ ПЕРЕГОРОДКИ (Добавили одной строчкой!)
        { 8_grid, 4_grid,  30,     4_grid }  // Внутренняя стена посреди комнаты
    };


    int m_doorGeometry[4] = {8_grid, 4_grid, static_cast<int>(1_grid * 0.4), static_cast<int>(1_grid * 1.1)};
    int m_safeZoneGeometry[4] = {7_grid, 14_grid, 2_grid, 1_grid};
    int m_safeGeometry[4] = {3_grid, 4_grid,  1_grid, 1_grid};


    // Константы размеров
    int m_playerStep = 1_grid;
    const int m_moveDuration = 300; // Скорость шага в миллисекундах

    const int m_mapSize = 1_grid * 16;   // Размер игрового поля
    const int m_playerSize = 16; // Размер кубика вора (ширина и высота)

    bool m_isMoving = false;
    QTimer *m_moveCooldownTimer; // Таймер блокировки кнопок на время анимации

    QTimer *m_timer; // Плюсовый таймер

    // Чистая C++ математика коллизий (AABB)
    bool checkCollision(int nx, int ny, int ox, int oy, int ow, int oh);
};
