#pragma once
#include "InteractableObject.h"

class CarObject: public InteractableObject
{
    Q_OBJECT

    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged FINAL)
    Q_PROPERTY(int state READ state WRITE setState NOTIFY stateChanged FINAL)

public:
    CarObject(int x, int y, int w, int h, QObject *parent = nullptr);

    bool onCollision(Character* actor) override;

    int x() const;
    void setX(int newX);

    int state() const { return m_state; }
    void setState(int newState);

    void arrive(int targetX);
    void escape(int targetX);
    Q_INVOKABLE void finishArrival();

signals:
    void xChanged();
    void stateChanged();
    void arrivalFinished();

private:
    int m_state = 0;
};
