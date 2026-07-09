import QtQuick

Rectangle {
    id: wall
    color: "#34495e"

    border.color: "#00d2d3"
    border.width: 1

    Text {
        text: "СТЕНА"
        color: "white"
        anchors.centerIn: parent
        font.pixelSize: 8
    }
}
