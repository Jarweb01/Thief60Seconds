import QtQuick

Rectangle {
    id: safeZone
    color: "transparent"
    border.color: "#3498db"
    border.width: 2

    Text {
        text: "ЗОНА"
        color: "#3498db"
        anchors.centerIn: parent
        font.pixelSize: 10
    }
}
