#pragma once
#include "InteractableObject.h"

class SafeObject: public InteractableObject
{
    Q_OBJECT

public:
    SafeObject(int x, int y, int w, int h, QObject *parent = nullptr);

    bool onCollision(Character* actor) override;
};
