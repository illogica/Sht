import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.0


Item {
    id: root
    visible: true
    width: 480
    height: 640
    property string name : "board"
    //title: qsTr("Hello World")
    property int maxCards: 5
    property int cardWidth: Math.min(height, width)/(maxCards + 1)
    property int cardHeight: cardWidth*2
    property variant cards : []

    function createCard(name){
        var c = Qt.createComponent("Card.qml");
        var card = c.createObject(root,
                                  {
                                      "finalX": randomX(),
                                      "finalY":randomY(),
                                      "height": cardHeight,
                                      "width": cardWidth,
                                      "name": name
                                  });
        if(card === null){
            console.log("Error creating card")
        } else {
            cards.push(card)
            card.cardClicked.connect(onCardClicked)
        }
    }

    function onCardClicked(obj){
        for(var i=0; i<cards.length; i++){
            cards[i].z -=0.1;
        }
        obj.z=1
    }

    function randomX(){
        var min = cardWidth
        var max = root.width - cardWidth
        return Math.random() * (max - min) + min;
    }

    function randomY(){
        var min = cardHeight
        var max = root.height - cardHeight
        return Math.random() * (max - min) + min;
    }

    function loadCards(){
        for(var i=0; i<maxCards; i++){
            var timer = Qt.createQmlObject("import QtQuick 2.0; Timer {}", root);
            timer.interval = i*50;
            timer.repeat = false;
            timer.triggered.connect(createCard);
            timer.start();
        }
    }

    Component.onCompleted: {
        //loadCards();
    }

    DropArea{
        id: dropArea
        height: root.height/3
        width: root.width/3

        anchors.right: parent.right
        anchors.top: parent.top

        onDropped: {
            var card = drag.source;
            Sht.sendPassCard(card.name)
            card.kill();
            card.destroy(500);
        }

        onContainsDragChanged: {
            if(containsDrag){
                background.border.width = 5
            } else {
                background.border.width = 0
            }

            /*var card = drag.source;
            card.killed = true;*/
        }

        Rectangle{
            id: background
            color: Qt.rgba(0.2,0.8,0.1,0.2)
            anchors.fill: parent
            radius: 10
            border.color: Qt.rgba(0.6,0.6,0.1,0.8)
            border.width : 0
        }
    }
}
