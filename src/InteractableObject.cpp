#include "InteractableObject.h"

InteractableObject::InteractableObject(int x, int y, int w, int h, const QString &type, bool state, QObject *parent)
    : QObject(parent), m_rect(x, y, w, h), m_type(type), m_isStateActive(state) {}

void InteractableObject::setIsStateActive(bool active){
    if (m_isStateActive == active) return;
    m_isStateActive = active;
    emit stateActiveChanged();
}
