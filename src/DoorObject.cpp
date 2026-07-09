#include "DoorObject.h"
#include <QDebug>

DoorObject::DoorObject(int x, int y, int w, int h, QObject * parent)
    : InteractableObject(x, y, w, h, "door", true, parent) {}

bool DoorObject::onCollision(Character * actor) {
    if (isStateActive()) {
        qDebug() << "C++: Вор врезался в запертую дверь! Отпираем...";

        setIsStateActive(false);

        return true;
    }

    return false;
}
