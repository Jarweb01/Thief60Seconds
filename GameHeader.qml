import QtQuick

Row {
    id: header
    height: 40
    spacing: 20

    // Входные свойства (пропсы)
    property int time: 0
    property string status: ""

    Text {
        text: "ВРЕМЯ: " + header.time
        color: "#f1c40f"
        font.pixelSize: 18
        font.bold: true
        anchors.verticalCenter: parent.verticalCenter
    }

    Text {
        text: header.status
        color: "white"
        font.pixelSize: 12
        anchors.verticalCenter: parent.verticalCenter
    }
}
