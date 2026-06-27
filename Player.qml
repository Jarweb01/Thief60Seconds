import QtQuick

// ИГРОК
Rectangle {
    id: player
    width: 16
    height: 16
    color: "#2ecc71"
    radius: 2

    Behavior on x { NumberAnimation { duration: gameEngine.moveDuration } }
    Behavior on y { NumberAnimation { duration: gameEngine.moveDuration } }
}
