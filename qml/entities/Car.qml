import QtQuick

Rectangle {
    id: car
    color: "black"
    border.color: "#00d2d3"
    border.width: 2

    y: gameEngine.car.rect.y
    width: gameEngine.car.rect.width
    height: gameEngine.car.rect.height

    property int stateIndex: 0

    x: gameEngine.carX

    // Плавная анимация изменения координаты X ровно на 2 секунды (2000 мс)
    Behavior on x {
        NumberAnimation {
            id: carAnimation
            duration: 2000
            easing.type: Easing.InOutQuad

            onRunningChanged: {
                if (!running) { // !running означает, что полет окончен!
                    console.log("QML: Машина ОСТАНОВИЛАСЬ в точке 400! Текущий stateIndex =", car.stateIndex);

                    if (car.stateIndex === 0) {
                        console.log("QML: Дергаем C++ метод GameEngine::onCarArrived()...");
                        gameEngine.onCarArrived();
                    }
                }
            }
        }
    }

    Text {
        text: stateIndex
        color: "white"
        anchors.centerIn: parent
        font.pixelSize: 10
    }
}
