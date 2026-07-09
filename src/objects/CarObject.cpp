#include "CarObject.h"
#include "entities/Character.h"
#include <QDebug>

CarObject::CarObject(int x, int y, int w, int h, QObject *parent)
    : InteractableObject(x, y, w, h, "car", false, parent) {}

bool CarObject::onCollision(Character* actor) {
    qDebug() << "C++: Персонаж наступил на машину. Сажаем внутрь!";

    actor->setIsInCar(true);

    // Машина НЕ должна блокировать движение (возвращаем false),
    // чтобы вор мог физически зайти на её клетки, и движок засчитал,
    // что персонаж находится "внутри машины" (isInCar = true).
    return false;
}
