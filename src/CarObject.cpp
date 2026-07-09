#include "CarObject.h"
#include <QDebug>

CarObject::CarObject(int x, int y, int w, int h, QObject *parent)
    : InteractableObject(x, y, w, h, "car", false, parent) {}

bool CarObject::onCollision(Character* actor) {
    qDebug() << "C++: Вор коснулся координат машины!";

    // Машина НЕ должна блокировать движение (возвращаем false),
    // чтобы вор мог физически зайти на её клетки, и движок засчитал,
    // что персонаж находится "внутри машины" (isInCar = true).
    return false;
}
