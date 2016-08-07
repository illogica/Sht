import QtQuick 2.5

Rectangle {
    id: root
    x: 10; y: 10
    width: 20; height: 20
    color: Qt.rgba(Math.random(), Math.random(), Math.random(), 1)

    property int finalX: 100
    property int finalY: 100
    property string name: "name"

    signal passed(string name)
    signal cardClicked(var obj)

    Drag.active: dragArea.drag.active
    Drag.hotSpot.x: 10
    Drag.hotSpot.y: 10

    function kill(){
        passAnim.start();
        passed(name);
    }

    Text{
        id: cardName
        text: name
        anchors.centerIn: root
        font.pixelSize: 20
    }

    Component.onCompleted: {
        xAnim.start()
        yAnim.start()
    }

    //Throw-in animations
    NumberAnimation on x{
        id: xAnim
        from: -100; to: finalX; duration: 500
        easing.type: Easing.OutQuart
    }
    //Throw-in animations
    NumberAnimation on y{
        id: yAnim
        from: -100; to: finalY; duration: 500
        easing.type: Easing.OutQuart
    }

    //Throw-out animations
    NumberAnimation on x {
        id: passAnim
        from: x;
        to: x+200;
        duration: 200
        easing.type: Easing.InQuart
    }

    MouseArea {
        id: dragArea
        anchors.fill: parent
        drag.target: parent
        onReleased: parent.Drag.drop()

        onPressed: {
            cardClicked(root)
        }
    }
}
