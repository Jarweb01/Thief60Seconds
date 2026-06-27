import QtQuick

// ИГРОК
Rectangle {
    id: player
    width: 16
    height: 16
    color: "#2ecc71"
    radius: 2

    Behavior on x { NumberAnimation { duration: 100 } }
    Behavior on y { NumberAnimation { duration: 100 } }
}
