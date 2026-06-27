import QtQuick

Rectangle {
    id: safe

    property bool isLooted: false

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
