#pragma once
#include "InteractableObject.h"

class CarObject: public InteractableObject
{
    Q_OBJECT

public:
    CarObject(int x, int y, int w, int h, QObject *parent = nullptr);

    bool onCollision(Character* actor) override;
};
