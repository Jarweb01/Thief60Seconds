import QtQuick

Rectangle {
    id: safe

    property var cppObject: null

    x: cppObject ? cppObject.rect.x : 0
    y: cppObject ? cppObject.rect.y : 0
    width: cppObject ? cppObject.rect.width : 0
    height: cppObject ? cppObject.rect.height : 0

    property bool isLooted: cppObject ? cppObject.isStateActive : false

    color:  isLooted ? "#f1c40f" : "#7f8c8d"
    border.color: "#00d2d3"
    border.width: 1

    Text {
        text: parent.isLooted ? 'ПУСТО' : 'CЕЙФ'
        color: parent.isLooted ? "black" : "white"
        anchors.centerIn: parent
        font.pixelSize: 10
    }

    // ИНДИКАТОР ВЗЛОМА СЕЙФА
    Rectangle {
        id: progressBarBg

        visible: safe.cppObject ? safe.cppObject.isBreaking : false

        // Позиционируем строго НАД сейфом
        anchors.bottom: parent.top
        anchors.bottomMargin: 4
        anchors.horizontalCenter: parent.horizontalCenter

        width: parent.width
        height: 6
        radius: 3
        color: "#34495e" // Темно-серый фон

        Rectangle {
            id: progressFill
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            radius: parent.radius
            color: "#f39c12" // Строительный оранжево-желтый цвет пиления сейфа

            width: parent.width * (safe.cppObject ? safe.cppObject.progress : 0.0)
        }
    }
}
