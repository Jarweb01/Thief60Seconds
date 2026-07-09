import QtQuick

Rectangle {
    id: door

    property var cppObject: null

    x: cppObject ? cppObject.rect.x : 0
    y: cppObject ? cppObject.rect.y : 0
    width: cppObject ? cppObject.rect.width : 0
    height: cppObject ? cppObject.rect.height : 0

    // Кастомный проп (стейт). По умолчанию дверь заперта (true)
    property bool isLocked: cppObject ? cppObject.isStateActive : true

    // Динамически меняем цвет: желтый если заперта, зеленый если открыта
    color: isLocked ? "#f1c40f" : "#27ae60"

    Text {
        text: parent.isLocked ? "ДВЕРЬ" : "ВХОД"
        color: parent.isLocked ? "black" : "white"
        anchors.centerIn: parent
        font.pixelSize: 9
    }

    // ИНДИКАТОР ВЗЛОМА (Шкала прогресса)
    Rectangle {
        id: progressBarBg

        visible: door.cppObject ? door.cppObject.isBreaking : false

        anchors.bottom: parent.top
        anchors.bottomMargin: 4
        anchors.horizontalCenter: parent.horizontalCenter

        width: parent.width
        height: 6
        radius: 3
        color: "#34495e"

        Rectangle {
            id: progressFill
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            radius: parent.radius
            color: "#1abc9c"

            width: parent.width * (door.cppObject ? door.cppObject.progress : 0.0)
        }
    }
}
