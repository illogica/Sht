import QtQuick 2.5
import QtGraphicalEffects 1.0

Item {
    id: root
    height: 100
    width: 100

    signal clicked();

    Image{
        id: image
        anchors.fill: parent
        source: "qrc:/emojione/2716.svg"
        sourceSize.width: image.width
        sourceSize.height: image.height
        mipmap: true
        smooth: true
    }

    MouseArea{
        id: mouseArea
        anchors.fill: parent
        onClicked: root.clicked()
        onPressed: overlay.color = "#80100000"
        onReleased: overlay.color = "#00000000"
    }

    ColorOverlay {
        id: overlay
        anchors.fill: image
        source: image
        color: "#00000000"
    }
}

