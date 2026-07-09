#pragma once
#include "InteractableObject.h"

class DoorObject: public InteractableObject
{
    Q_OBJECT

public:
    DoorObject(int x, int y, int w, int h, QObject * parent = nullptr);

    bool onCollision(Character * actor) override;
};

