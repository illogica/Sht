import QtQuick 2.7
import QtQuick.Controls 2.0

Rectangle {
    id: root
    width: 640
    height: 480
    visible: true
    color: Qt.rgba(0.2,0.1,0.1, 0.1)

    ListView {
        id: listView
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10
        model: matchesModel
        delegate: ItemDelegate {
            text: "Partita di " + ownerName + ", " + playersNumber + " giocatori. " + (matchState == 0 ? "Puoi partecipare!" : "Partita in corso")
            width: listView.width - listView.leftMargin - listView.rightMargin
            background: Rectangle{
                color: Qt.rgba(Math.random(),Math.random(),Math.random(), 0.2)
             }
            //height: avatar.implicitHeight
            //leftPadding: avatar.implicitWidth + 32
            onClicked: {
                console.log("Clicked: " + uuid)
                if(matchState == 0)
                    Sht.sendJoinMatch(uuid)
                else
                    console.log("Cannot join the match with state " + matchState);
            }
        }
    }
}
