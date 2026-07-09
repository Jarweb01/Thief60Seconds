import QtQuick
import "../components"

Window {
    id: root
    width: 800
    height: 800
    visible: true
    title: "2D Прототип за 60 секунд"
    color: gameEngine.timeManager.isTimeRunningOut ? "#59212D" : "#1a252f"

    Behavior on color {
        // Анимация сработает только тогда, когда isTimeRunningOut переключится в true
        SequentialAnimation {
            loops: Animation.Infinite // Мигаем бесконечно, пока не наступит GameOver

            ColorAnimation { duration: 600; easing.type: Easing.InOutQuad }
            ColorAnimation { to: "#1a252f"; duration: 600; easing.type: Easing.InOutQuad }
        }
    }

    // ВЕРХНЯЯ ПАНЕЛЬ СЧЕТЧИКА
    GameHeader {
        id: header
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        time: gameEngine.timeManager.timeLeft       // Передаем стейт времени
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

        // КНОПКА СТАРТА ИГРЫ
        Rectangle {
            id: startButton

            // Центрируем кнопку строго посреди игрового поля
            anchors.centerIn: parent
            width: 200
            height: 60
            radius: 8

            // Стильный темно-зеленый цвет
            color: "#27ae60"
            border.color: "#2ecc71"
            border.width: 2

            // Эффект плавного исчезновения при старте
            Behavior on opacity { NumberAnimation { duration: 250 } }

            Text {
                anchors.centerIn: parent
                text: "НАЧАТЬ ИГРУ"
                color: "white"
                font.bold: true
                font.pixelSize: 18
            }

            // mouseArea делает прямоугольник кликабельным
            MouseArea {
                anchors.fill: parent
                // Меняем курсор на "руку" при наведении
                cursorShape: Qt.PointingHandCursor

                onClicked: {
                    console.log("QML: Нажата кнопка СТАРТ! Пинаем C++ движок...");

                    // 1. Вызываем наш новый плюсовый метод
                    gameEngine.startLevel();

                    // 2. Делаем кнопку невидимой (сработает плавная анимация затухания)
                    startButton.opacity = 0

                    // 3. Отключаем кликабельность, чтобы нельзя было прожать повторно
                    startButton.visible = false
                }
            }
        }

        Player {
            id: player
            x: gameEngine.player.x
            y: gameEngine.player.y

            opacity: gameEngine.player.isInCar ? 0 : 1

            Behavior on opacity { NumberAnimation { duration: 100 } }
        }

        // НАПАРНИК (Ассистент)
        Player {
            id: assistant
            x: gameEngine.assistant.x
            y: gameEngine.assistant.y

            // Делаем его полупрозрачным или меняем цвет, чтобы отличить от главного вора
            opacity: gameEngine.assistant.isInCar ? 0 : 0.7

            Behavior on opacity { NumberAnimation { duration: 100 } }

            // Если внутри вашего Player.qml цвет задан через property,
            // можно перекрасить его, например: color: "#3498db" (синий)
        }

        // СТЕНЫ
        Repeater {
            model: gameEngine.map.walls
            Wall {
                x: modelData.x
                y: modelData.y
                width: modelData.w
                height: modelData.h
            }
        }

        // ДИНАМИЧЕСКИЕ ИГРОВЫЕ ОБЪЕКТЫ (Машина, Дверь, Сейф)
        Repeater {
            // Читаем QVariantList вектор из C++
            model: gameEngine.gameObjects

            // Loader — это стандартный компонент QML, который умеет динамически
            // загружать разные файлы (.qml) в зависимости от условий
            Loader {
                id: objectLoader

                // Настраиваем логику выбора правильного файла по текстовому типу из C++
                source: {
                    if (modelData.type === "car")  return "Car.qml"
                    if (modelData.type === "door") return "Door.qml"
                    if (modelData.type === "safe") return "Safe.qml"
                    return ""
                }

                // Связываем динамически созданный объект со свойствами из C++
                onLoaded: {
                    // Передаем ссылку на C++ объект внутрь загруженного QML-файла
                    // Каждая Дверь, Сейф или Машина теперь получит свой персональный "мозг"
                    item.cppObject = modelData
                }
            }
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
