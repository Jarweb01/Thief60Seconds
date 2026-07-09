import QtQuick

Rectangle {
    id: car
    color: "black"
    border.color: "#00d2d3"
    border.width: 2

    property var cppObject: null

    y: cppObject ? cppObject.rect.y : 0
    width: cppObject ? cppObject.rect.width : 0
    height: cppObject ? cppObject.rect.height : 0

    property int stateIndex: cppObject ? cppObject.state : 0
    x: cppObject ? cppObject.x : -50

    // Плавная анимация изменения координаты X ровно на 2 секунды (2000 мс)
    Behavior on x {
        enabled: car.cppObject !== null

        NumberAnimation {
            id: carAnimation
            duration: 2000
            easing.type: Easing.InOutQuad

            onRunningChanged: {
                if (!running) { // !running означает, что полет окончен!
                    console.log("QML: Машина ОСТАНОВИЛАСЬ в точке " + car.x + "! Текущий stateIndex =", car.stateIndex);

                    if (car.stateIndex === 0 && car.x > 0 && car.cppObject) {
                        console.log("QML: Дергаем C++ метод car.cppObject.finishArrival()...");
                        car.cppObject.finishArrival();
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
