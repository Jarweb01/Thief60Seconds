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
}
