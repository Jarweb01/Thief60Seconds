import QtQuick

Window {
    id: root
    width: 800
    height: 800
    visible: true
    title: "2D Прототип за 60 секунд"
    color: "#1a252f"

    // ВЕРХНЯЯ ПАНЕЛЬ СЧЕТЧИКА
    GameHeader {
        id: header
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        time: gameEngine.timeLeft       // Передаем стейт времени
        status: gameEngine.gameStatus   // Передаем стейт статуса
    }

    // Карта (Игровое поле)
    Item {
        id: map
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        focus: true

        GameGrid {}

        // БЕЗОПАСНАЯ ЗОНА
        SafeZone {
            id: safeZone
            x: gameEngine.safeZoneX
            y: gameEngine.safeZoneY
            width: gameEngine.safeZoneWidth
            height: gameEngine.safeZoneHeight
        }

        // Автоматическая отрисовка всех стен комнаты из C++ массива
        Repeater {
            model: gameEngine.walls
            Wall {
                x: modelData.x
                y: modelData.y
                width: modelData.w
                height: modelData.h
            }
        }

        // СТЕНА
        Wall {
            id: wall
            x: gameEngine.wallX
            y: gameEngine.wallY
            width: gameEngine.wallWidth
            height: gameEngine.wallHeight
        }

        // ДВЕРЬ
        Door {
            id: door
            x: gameEngine.doorX
            y: gameEngine.doorY
            width: gameEngine.doorWidth
            height: gameEngine.doorHeight
            isLocked: gameEngine.doorLocked
        }

        // СЕЙФ С ДОБЫЧЕЙ
        Safe {
            id: safe
            x: gameEngine.safeX
            y: gameEngine.safeY
            width: gameEngine.safeWidth
            height: gameEngine.safeHeight
            isLooted: gameEngine.safeLooted
        }

        Player {
            id: player
            x: gameEngine.playerX
            y: gameEngine.playerY
        }

        // Логика управления (чистый JS)
        Keys.onPressed: (event) => {
            if (event.key === Qt.Key_Left)  { gameEngine.handleKeyPress("Left") }
            if (event.key === Qt.Key_Right) { gameEngine.handleKeyPress("Right") }
            if (event.key === Qt.Key_Up)    { gameEngine.handleKeyPress("Up") }
            if (event.key === Qt.Key_Down)  { gameEngine.handleKeyPress("Down") }
        }
    }
}
