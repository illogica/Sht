import QtQuick 2.7
import QtQuick.Controls 2.0

import "components" 1.0

Pane {
    id: root
    height: 640
    width: 480
    property string uuid: ""

    //Component.onCompleted: Sht.sendGetMatches()
    function createCard(card){
        board.createCard(card);
    }

    function appendChat(txt){
        chatText.text = chatText.text + txt
    }


    Label {
        id:labelTitle
        width: parent.width
        wrapMode: Label.Wrap
        horizontalAlignment: Qt.AlignHCenter
        text: "Partita"
    }

    Button {
        id: buttonStartMatch
        anchors { horizontalCenter: parent.horizontalCenter; top: labelTitle.bottom; topMargin: 20 }
        text: "Inizia la partita!"
        //width: itemWidth
        onClicked:{
            Sht.sendStartMatch();
            console.log("Called startMatch()");
        }
    }

    Board{
        id: board
        anchors.top: labelTitle.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }

    Flickable{
        id: chatArea
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width/2
        height: parent.height/2
        contentHeight: chatText.height

        Text{
            id: chatText
            text: "Buongiorno"
            width: chatArea.width
            wrapMode: Text.WordWrap
        }
    }
}

//ScrollIndicator.vertical: ScrollIndicator { }

