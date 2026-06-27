import QtQuick

Rectangle {
    id: door

    // Кастомный проп (стейт). По умолчанию дверь заперта (true)
    property bool isLocked: true

    // Динамически меняем цвет: желтый если заперта, зеленый если открыта
    color: isLocked ? "#f1c40f" : "#27ae60"

    Text {
        text: parent.isLocked ? "ДВЕРЬ" : "ВХОД"
        color: parent.isLocked ? "black" : "white"
        anchors.centerIn: parent
        font.pixelSize: 9
    }
}
