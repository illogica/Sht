import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.0

ApplicationWindow {
    id: root
    width: 540
    height: 960
    visible: true

    Page {
        anchors.fill: parent
        header: Label {
            padding: 10
            text: qsTr("Players")
            font.pixelSize: 20
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        ListView {
            id: listView
            anchors.fill: parent
            topMargin: 48
            leftMargin: 48
            bottomMargin: 48
            rightMargin: 48
            spacing: 20
            model: ["Albert Einstein", "Ernest Hemingway", "Hans Gude"]
            delegate: ItemDelegate {
                text: modelData
                width: listView.width - listView.leftMargin - listView.rightMargin
                height: avatar.implicitHeight
                leftPadding: avatar.implicitWidth + 32

                Image {
                    id: avatar
                    source: "qrc:/" + modelData + ".png"
                }
            }
        }
    }
}
