#pragma once
#include <QObject>
#include <QRect>
#include <QString>

class InteractableObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QRect rect READ rect CONSTANT)
    Q_PROPERTY(QString type READ type CONSTANT)
    Q_PROPERTY(bool isStateActive READ isStateActive WRITE setIsStateActive NOTIFY stateActiveChanged FINAL)

public:
    explicit InteractableObject(int x, int y, int w, int h, const QString & type, bool state, QObject *parent = nullptr);

    QRect rect() const { return m_rect; }
    QString type() const { return m_type; }

    bool isStateActive() const { return m_isStateActive; }
    void setIsStateActive(bool active);

signals:
    void stateActiveChanged();

private:
    QRect m_rect;
    QString m_type;
    bool m_isStateActive;
};
