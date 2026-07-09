#pragma once
#include "InteractableObject.h"
#include <QTimer>

class Character;

class SafeObject: public InteractableObject
{
    Q_OBJECT

    Q_PROPERTY(double progress READ progress NOTIFY progressChanged FINAL)
    Q_PROPERTY(bool isBreaking READ isBreaking NOTIFY isBreakingChanged FINAL)

public:
    SafeObject(int x, int y, int w, int h, QObject *parent = nullptr);

    bool onCollision(Character* actor) override;

    double progress() const { return m_progress; }
    bool isBreaking() const { return m_isBreaking; }

signals:
    void progressChanged();
    void isBreakingChanged();
    void safeLooted();

private slots:
    void onCrackTick();

private:
    double m_progress = 0.0;
    bool m_isBreaking = false;

    QTimer* m_crackTimer = nullptr;
    Character* m_currentActor = nullptr;

    int m_currentCrackTime = 0;
    const int m_totalCrackTime = 2000;
};
