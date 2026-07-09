#include "SafeObject.h"
#include "entities/Character.h"
#include <QDebug>

SafeObject::SafeObject(int x, int y, int w, int h, QObject *parent)
    : InteractableObject(x, y, w, h, "safe", false, parent)
{
    m_crackTimer = new QTimer(this);
    m_crackTimer->setInterval(TIMER_INTERVAL);
    connect(m_crackTimer, &QTimer::timeout, this, &SafeObject::onCrackTick);
}


bool SafeObject::onCollision(Character* actor) {
    // Если сейф еще НЕ взломан (isStateActive == false)
    if (!isStateActive() && !m_isBreaking) {
        qDebug() << "C++: Вор добрался до сейфа! Вскрываем и забираем лут...";

        m_isBreaking = true;
        m_progress = 0.0;
        m_currentCrackTime = 0;

        m_currentActor = actor;
        m_currentActor->setBusy(true);

        emit isBreakingChanged();
        emit progressChanged();

        m_crackTimer->start();
        return true;
    }

    if (m_isBreaking) {
        return true;
    }

    // Сейф — это твердое препятствие, сквозь него ходить нельзя.
    // Поэтому ВСЕГДА возвращаем true, блокируя шаг вора.
    return true;
}

void SafeObject::onCrackTick() {
    m_currentCrackTime += TIMER_INTERVAL;
    m_progress = static_cast<double>(m_currentCrackTime) / m_totalCrackTime;
    emit progressChanged();

    if (m_currentCrackTime >= m_totalCrackTime) {
        m_crackTimer->stop();
        m_isBreaking = false;
        m_progress = 1.0;

        setIsStateActive(true);

        if (m_currentActor) {
            m_currentActor->setBusy(false);
            m_currentActor = nullptr;
        }

        emit isBreakingChanged();
        emit progressChanged();
        emit safeLooted();

        qDebug() << "C++: Сейф успешно обчищен!";
    }
}
