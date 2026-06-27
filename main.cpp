#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext> // нужен для проброса C++ объектов в QML
#include "gameengine.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    GameEngine gameEngine;
    engine.rootContext()->setContextProperty("gameEngine", &gameEngine);


    const QUrl url(QStringLiteral("qrc:/Thief60Seconds/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
