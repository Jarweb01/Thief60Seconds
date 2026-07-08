import QtQuick

Rectangle {
    id: safe

    x: gameEngine.safe.rect.x
    y: gameEngine.safe.rect.y
    width: gameEngine.safe.rect.width
    height: gameEngine.safe.rect.height

    property bool isLooted: gameEngine.safe.isStateActive

    color:  isLooted ? "#f1c40f" : "#7f8c8d"

    border.color: "#00d2d3"
        border.width: 1

    Text {
        text: parent.isLooted ? 'ПУСТО' : 'CЕЙФ'
        color: parent.isLooted ? "black" : "white"
        anchors.centerIn: parent
        font.pixelSize: 10
    }
}
