import QtQuick

Rectangle {
    id: door

    x:gameEngine.door.rect.x
    y: gameEngine.door.rect.y
    width: gameEngine.door.rect.width
    height: gameEngine.door.rect.height

    // Кастомный проп (стейт). По умолчанию дверь заперта (true)
    property bool isLocked: gameEngine.door.isStateActive

    // Динамически меняем цвет: желтый если заперта, зеленый если открыта
    color: isLocked ? "#f1c40f" : "#27ae60"

    Text {
        text: parent.isLocked ? "ДВЕРЬ" : "ВХОД"
        color: parent.isLocked ? "black" : "white"
        anchors.centerIn: parent
        font.pixelSize: 9
    }
}
