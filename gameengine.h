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

public:
    explicit GameEngine(QObject *parent = nullptr);

    // Геттеры для чтения данных из QML
    int timeLeft() const { return m_timeLeft; }
    bool isGameOver() const { return m_isGameOver; }
    bool doorLocked() const { return m_doorLocked; }
    QString gameStatus() const { return m_gameStatus; }
    int playerX() const { return m_playerX; }
    int playerY() const { return m_playerY; }

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

    int m_playerX = 192;
    int m_playerY = 347;

    QTimer *m_timer; // Плюсовый таймер

    // Чистая C++ математика коллизий (AABB)
    bool checkCollision(int nx, int ny, int ox, int oy, int ow, int oh);
};
