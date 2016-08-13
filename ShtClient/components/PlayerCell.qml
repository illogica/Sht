import QtQuick 2.7

Rectangle {
    id: root

    height: 200
    width: 200

    property string avatar: "1f33c"
    property string playerName: "nome"
    property  int kilos: 0

    visible: true
    color: Qt.rgba(Math.random(), Math.random(), Math.random(), 0.1)
    radius: 15

    Image{
        id: image
        anchors.fill: parent
        anchors.margins: 10
        source: "qrc:/emojione/" + avatar + ".svg"
        sourceSize.width: image.width
        sourceSize.height: image.height
        mipmap: true
        smooth: true
    }

    Text{
        id: nameText
        anchors.left: parent.left
        anchors.top: parent.top
        text: playerName
    }

    Text{
        id: kilosText
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        text: "Kili: " + kilos
    }
}
