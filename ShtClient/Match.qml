import QtQuick 2.7
import QtQuick.Controls 2.0

import "components" 1.0

Pane {
    id: root
    height: 640
    width: 480
    property string uuid: ""

    state: "init"
    onStateChanged: console.log("new state: " + state)

    states: [
        State {
            name: "init"
            PropertyChanges {target: buttonStartMatch; visible: true}
            PropertyChanges {target: buttonSht; visible:false}
        },
        State {
            name: "started"
            PropertyChanges {target: buttonStartMatch; visible: false}
            PropertyChanges {target: buttonSht; visible:true}
        }
    ]

    //Component.onCompleted: Sht.sendGetMatches()
    function createCard(card){
        board.createCard(card);
    }

    function appendChat(txt){
        chatText.text = chatText.text + "\n" + txt
    }

    function reset(){
        state = "init"
        board.reset()
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
        background: Rectangle {
            id: buttonStartBackground
            anchors.fill: parent
            color: "palegreen"
            radius: 4
        }
        onClicked:{
            Sht.sendStartMatch();
        }
    }


    Button {
        id: buttonSht
        anchors { horizontalCenter: parent.horizontalCenter; top: labelTitle.bottom; topMargin: 20 }
        text: "MERDA!"
        //width: itemWidth
        background: Rectangle {
            id: buttonShtBackground
            anchors.fill: parent
            color: "goldenrod"
            radius: 4
        }
        onClicked:{
            Sht.sendSht();
            console.log("Called MERDA!");
        }
    }

    Board{
        id: board
        anchors.top: labelTitle.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }

    ListView{
        id: listChat
        model: chatModel
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width/3
        height: parent.height/4
        z:-1
        currentIndex: model.count - 1

        delegate: Text{
            id: chatText
            text: model.message
            wrapMode: Text.WordWrap
        }

        ScrollIndicator.vertical: ScrollIndicator { }
    }

    GridView{
        id: gridViewPlayers
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: root.height/4

        model: playersModel
        delegate: PlayerCell{
            height: 80
            width: 80
            playerName: plrName
            kilos: plrKilos
            avatar: plrAvatar
        }
    }

    CloseButton{
        id: closeButton
        anchors.right: parent.right
        anchors.top: parent.top
        height: 30
        width: 30
        onClicked:{
            stack.pop();
            Sht.sendLeaveMatch();
        }
    }
}

//ScrollIndicator.vertical: ScrollIndicator { }

