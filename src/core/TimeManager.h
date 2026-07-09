#pragma once
#include <QObject>
#include <QTimer>

class TimeManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int timeLeft READ timeLeft NOTIFY timeLeftChanged FINAL)
    Q_PROPERTY(bool isTimeRunningOut READ isTimeRunningOut NOTIFY isTimeRunningOutChanged FINAL)
public:
    explicit TimeManager(QObject *parent = nullptr);

    int timeLeft() const { return m_timeLeft; }
    bool isTimeRunningOut() const { return m_timeLeft < m_panicThreshold && m_timeLeft > 0; }

    void stop();
    void start();

signals:
    void timeLeftChanged();
    void isTimeRunningOutChanged();
    void timeUp();

private slots:
    void onTimerTick();

private:
    int m_timeLeft;
    QTimer * m_timer = nullptr;
    const int m_panicThreshold = 20;
};

