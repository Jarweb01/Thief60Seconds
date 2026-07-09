#include "DoorObject.h"
#include "entities/Character.h"
#include <QDebug>

DoorObject::DoorObject(int x, int y, int w, int h, QObject * parent)
    : InteractableObject(x, y, w, h, "door", true, parent) {
    m_breakTimer = new QTimer(this);
    m_breakTimer->setInterval(50);
    connect(m_breakTimer, &QTimer::timeout, this, &DoorObject::onLockPickTick);
}


bool DoorObject::onCollision(Character * actor) {
    if (isStateActive() && !m_isBreaking) {
        qDebug() << "C++: Вор врезался в запертую дверь! Отпираем...";

        m_isBreaking = true;
        m_progress = 0.0;
        m_currentBreakTime = 0;

        m_currentActor = actor;
        m_currentActor->setBusy(true);

        emit isBreakingChanged();
        emit progressChanged();

        m_breakTimer->start();

        return true;
    }

    if (m_isBreaking) {
        return true;
    }

    return false;
}

void DoorObject::onLockPickTick() {
    m_currentBreakTime += TIMER_INTERVAL;

    m_progress = static_cast<double>(m_currentBreakTime) / m_totalBreakTime;
    emit progressChanged();

    if (m_currentBreakTime >= m_totalBreakTime) {
        m_breakTimer->stop();
        m_isBreaking = false;
        m_progress = 1.0;

        setIsStateActive(false);

        if (m_currentActor) {
            m_currentActor->setBusy(false);
            m_currentActor = nullptr;
        }

        emit isBreakingChanged();
        emit progressChanged();
        emit doorOpened();

        qDebug() << "C++: Дверь успешно взломана!";
    }
}
