#include "SafeObject.h"
#include <QDebug>

SafeObject::SafeObject(int x, int y, int w, int h, QObject *parent)
    : InteractableObject(x, y, w, h, "safe", false, parent) {}


bool SafeObject::onCollision(Character* actor) {
    // Если сейф еще НЕ взломан (isStateActive == false)
    if (!isStateActive()) {
        qDebug() << "C++: Вор добрался до сейфа! Вскрываем и забираем лут...";

        setIsStateActive(true); // Переключаем в true (разграблен, в QML он станет золотым)
    }

    // Сейф — это твердое препятствие, сквозь него ходить нельзя.
    // Поэтому ВСЕГДА возвращаем true, блокируя шаг вора.
    return true;
}
