import QtQuick

Rectangle {
    id: safe

    property var cppObject: null

    x: cppObject ? cppObject.rect.x : 0
    y: cppObject ? cppObject.rect.y : 0
    width: cppObject ? cppObject.rect.width : 0
    height: cppObject ? cppObject.rect.height : 0

    property bool isLooted: cppObject ? cppObject.isStateActive : false

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
