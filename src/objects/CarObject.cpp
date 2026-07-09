#include "CarObject.h"
#include "entities/Character.h"
#include <QDebug>

CarObject::CarObject(int x, int y, int w, int h, QObject *parent)
    : InteractableObject(x, y, w, h, "car", false, parent), m_state(0) {}

int CarObject::x() const {
    return m_rect.x();
}

void CarObject::setX(int newX) {
    if (rect().x() == newX) return;

    m_rect.moveTo(newX, m_rect.y());
    emit xChanged();
}

void CarObject::setState(int newState) {
    if (m_state == newState) return;
    m_state = newState;
    emit stateChanged();
}

bool CarObject::onCollision(Character* actor) {
    qDebug() << "C++: Персонаж наступил на машину. Сажаем внутрь!";

    actor->setIsInCar(true);

    // Машина НЕ должна блокировать движение (возвращаем false),
    // чтобы вор мог физически зайти на её клетки, и движок засчитал,
    // что персонаж находится "внутри машины" (isInCar = true).
    return false;
}

void CarObject::arrive(int targetX) {
    setState(0);
    setX(targetX);
}

void CarObject::escape(int targetX) {
    setState(2);
    setX(targetX);
}

void CarObject::finishArrival() {
    qDebug() << "C++ Машина: Анимация завершена, переходим в режим ожидания.";
    setState(1);
    emit arrivalFinished();
}
