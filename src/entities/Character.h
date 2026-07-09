#pragma once
#include <QObject>

class Character: public QObject
{
    Q_OBJECT

    // Регистрируем свойства, которые QML сможет читать у конкретного персонажа
    Q_PROPERTY(int x READ x WRITE setX NOTIFY positionChanged FINAL)
    Q_PROPERTY(int y READ y WRITE setY NOTIFY positionChanged FINAL)
    Q_PROPERTY(bool isInCar READ isInCar WRITE setIsInCar NOTIFY isInCarChanged FINAL)
    Q_PROPERTY(int moveDuration READ moveDuration WRITE setMoveDuration NOTIFY moveDurationChanged FINAL)

public:
    explicit Character(int startX, int startY, QObject *parent = nullptr);

    // Геттеры и сеттеры
    int x() const { return m_x; }
    void setX(int newX);

    int y() const { return m_y; }
    void setY(int newY);

    bool isInCar() const { return m_isInCar; }
    void setIsInCar(bool inCar);

    int moveDuration() const { return m_moveDuration; }
    void setMoveDuration(int duration);

    int size() const { return m_size; }

    bool isBusy() const { return m_isBusy; }
    void setBusy(bool busy) { m_isBusy = busy; }

signals:
    void positionChanged();
    void isInCarChanged();
    void moveDurationChanged();

private:
    int m_x;
    int m_y;
    int m_size = 16;
    int m_moveDuration = 300;
    bool m_isInCar = true;
    bool m_isBusy = false;
};
