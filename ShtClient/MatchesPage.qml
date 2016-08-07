import QtQuick 2.7
import QtQuick.Controls 2.0

import "components" 1.0

Pane {
    id: root
    height: 640
    width: 480

    Component.onCompleted: Sht.sendGetMatches()

    Label {
        id:labelTitle
        width: parent.width
        wrapMode: Label.Wrap
        horizontalAlignment: Qt.AlignHCenter
        text: "Scegli una partita o creane una nuova!"
    }

    Button {
        id: buttonCreateMatch
        anchors { horizontalCenter: parent.horizontalCenter; top: labelTitle.bottom; topMargin: 20 }
        text: "Crea nuova partita!"
        //width: itemWidth
        onClicked:{
            Sht.sendCreateMatch();
            console.log("Called addMatch()");
        }
    }

    MatchesList{
        id: matchesList
        anchors.top: buttonCreateMatch.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
}

//ScrollIndicator.vertical: ScrollIndicator { }

