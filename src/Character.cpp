#include "Character.h"

Character::Character(int startX, int startY, QObject *parent): QObject(parent), m_x(startX), m_y(startY) {}

void Character::setX(int newX) {
    if (m_x == newX) return;
    m_x = newX;
    emit positionChanged();
}

void Character::setY(int newY) {
    if (m_y == newY) return;
    m_y = newY;
    emit positionChanged();
}

void Character::setIsInCar(bool inCar) {
    if(m_isInCar == inCar) return;
    m_isInCar = inCar;
    emit isInCarChanged();
}

void Character::setMoveDuration(int duration) {
    if (m_moveDuration == duration) return;
    m_moveDuration = duration;
    emit moveDurationChanged();
}
