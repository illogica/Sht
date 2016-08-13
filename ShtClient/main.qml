import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Extras 1.4
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import Qt.labs.settings 1.0

import "components" 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 480
    height: 640
    title: qsTr("Merda!")

    Component.onCompleted:{
        Sht.active = true;
    }

    Settings {
        id: settings
        property string style: "Universal"
    }

    header: ToolBar {
        Material.foreground: "white"

        RowLayout {
            spacing: 20
            anchors.fill: parent

            ToolButton {
                contentItem: Image {
                    id: drawerButton
                    height: 50
                    width: 50
                    //fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/images/drawer.svg"
                    sourceSize.width: drawerButton.width
                    sourceSize.height: drawerButton.height
                    mipmap: true
                    smooth: true
                }
                onClicked: drawer.open()
            }

            RowLayout {
                //horizontalAlignment: Qt.AlignHCenter
                //verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true

                StatusIndicator{
                    id: statusIndicator
                    color: "skyblue"
                    active: Sht.connected
                    height: titleLabel.height/2 //Style.headerHeight - 5
                    width: height
                    anchors.margins: 5
                }

                Label {
                    id: titleLabel
                    text: "Merda!"
                    font.pixelSize: 20
                    elide: Label.ElideRight
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    Layout.fillWidth: true
                }
            }

            ToolButton {
                contentItem: Image {
                    id: menuImage
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/images/menu.svg"
                    sourceSize.width: menuImage.width
                    sourceSize.height: menuImage.height
                    mipmap: true
                    smooth: true
                }
                onClicked: optionsMenu.open()

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    MenuItem {
                        text: "Settings"
                        onTriggered: settingsPopup.open()
                    }
                    MenuItem {
                        text: "About"
                        onTriggered: aboutDialog.open()
                    }
                }
            }
        }
    }

    Drawer {
        id: drawer
        width: Math.min(root.width, root.height) / 3 * 2
        height: root.height

        ListView {
            id: listView
            currentIndex: -1
            anchors.fill: parent

            delegate: ItemDelegate {
                width: parent.width
                text: model.title
                highlighted: ListView.isCurrentItem
                onClicked: {
                    if (listView.currentIndex != index) {
                        listView.currentIndex = index
                        titleLabel.text = model.title
                        //stack.push(model.source)
                        stack.loadById(menuId)
                        console.log("stack.push " + model.menuId);
                    }
                    drawer.close()
                }
            }

            model: ListModel {
                //ListElement { title: qsTr("Matches"); source: "qrc:/MatchesPage.qml" }
                ListElement { title: qsTr("Matches");  menuId: 0}
                ListElement { title: qsTr("Players"); source: "qrc:/pages/ButtonPage.qml" }
                ListElement { title: qsTr("Your Stats"); source: "qrc:/pages/CheckBoxPage.qml" }
            }

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }

    StackView {
        id: stack
        anchors.fill: parent

        function loadById(id){
            switch(id){
            case 0:
                Sht.sendGetMatches()
                stack.push(matchesPage);
                break;
            default:
                console.log("Work better");
            }
        }

        initialItem: Pane {
            id: pane

            Image {
                id: logo
                width: pane.availableWidth / 2
                height: pane.availableHeight / 2
                anchors.centerIn: parent
                anchors.verticalCenterOffset: -50
                fillMode: Image.PreserveAspectFit
                source: "qrc:/emojione/1f4a9.svg"
                sourceSize.width: logo.width
                sourceSize.height: logo.height
                mipmap: true
                smooth: true
            }

            Label {
                text: "Giochi di carte popolari: Merda!"
                anchors.margins: 20
                anchors.top: logo.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                wrapMode: Label.Wrap
            }
        }
    }

    Popup {
        id: settingsPopup
        x: (root.width - width) / 2
        y: root.height / 6
        width: Math.min(root.width, root.height) / 3 * 2
        height: settingsColumn.implicitHeight + topPadding + bottomPadding
        modal: true
        focus: true

        contentItem: ColumnLayout {
            id: settingsColumn
            spacing: 20

            Label {
                text: "Settings"
                font.bold: true
            }

            RowLayout {
                spacing: 10

                Label {
                    text: "Style:"
                }

                ComboBox {
                    id: styleBox
                    property int styleIndex: -1
                    model: ["Default", "Material", "Universal"]
                    Component.onCompleted: {
                        styleIndex = find(settings.style, Qt.MatchFixedString)
                        if (styleIndex !== -1)
                            currentIndex = styleIndex
                    }
                    Layout.fillWidth: true
                }
            }

            Label {
                text: "Restart required"
                color: "#e41e25"
                opacity: styleBox.currentIndex !== styleBox.styleIndex ? 1.0 : 0.0
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            RowLayout {
                spacing: 10

                Button {
                    id: okButton
                    text: "Ok"
                    onClicked: {
                        settings.style = styleBox.displayText
                        settingsPopup.close()
                    }

                    Material.foreground: Material.primary
                    Material.background: "transparent"
                    Material.elevation: 0

                    Layout.preferredWidth: 0
                    Layout.fillWidth: true
                }

                Button {
                    id: cancelButton
                    text: "Cancel"
                    onClicked: {
                        styleBox.currentIndex = styleBox.styleIndex
                        settingsPopup.close()
                    }

                    Material.background: "transparent"
                    Material.elevation: 0

                    Layout.preferredWidth: 0
                    Layout.fillWidth: true
                }
            }
        }
    }

    Popup {
        id: aboutDialog
        modal: true
        focus: true
        x: (root.width - width) / 2
        y: root.height / 6
        width: Math.min(root.width, root.height) / 3 * 2
        contentHeight: aboutColumn.height

        Column {
            id: aboutColumn
            spacing: 20

            Label {
                text: "About"
                font.bold: true
            }

            Label {
                width: aboutDialog.availableWidth
                text: "Inserire qualche testo sensato qui"
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }

            Label {
                width: aboutDialog.availableWidth
                text: "Inserire qualche altro testo sensato"
                      + "anche se non saprei cosa scrivere. "
                      + "Cazzo."
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }
        }
    }

    //QML OBJECTS
    ListModel{
        id: matchesModel
        objectName: "matchesModel"
        //property int page: 0

        function addMatch(ownerName, playersNumber, matchState, uuid){
            matchesModel.append({
                                     "ownerName": ownerName,
                                     "playersNumber": playersNumber,
                                     "uuid": uuid,
                                     "matchState": matchState
                                     })
        }
        onCountChanged: {
            if(count === 0)
                console.log("Empty matches list");
        }

        function reset(){
            clear()
            //page = 0;
        }
    }

    ListModel{
        id: chatModel
        objectName: "chatModel"

        function addText(message){
            chatModel.append({"message": message});
        }

        function reset(){
            clear()
            console.log("cleared chat messages");
        }
    }

    ListModel{
        id: playersModel
        objectName: "playersModel"

        function addPlayer(name, kilos, avatar){
            playersModel.append({
                                    "plrName": name,
                                    "plrKilos": kilos,
                                    "plrAvatar" : avatar
                                });
        }

        function reset(){
            clear()
            console.log("cleared players model");
        }
    }

    Match{
        id: match
        visible: false
    }

    MatchesPage{
        id: matchesPage
        visible: false
    }

    Connections {
        target: Sht
        onQmlAddMatch: {
            matchesModel.addMatch(ownerName, playersNumber, matchState, uuid);
        }
        onQmlResetMatches: {
            matchesModel.reset();
        }
        onQmlOpenMatch: {
            //stack.push("qrc:/Match.qml")
            stack.push(match)
        }
        onQmlAddChatMessage: {
            chatModel.addText(message);
            //match.appendChat(message)
        }
        onQmlChatReset: {
            chatModel.reset();
        }
        onQmlHaveCard: {
            match.createCard(card)
        }
        onQmlAddPlayerToModel: {
            playersModel.addPlayer(name, kilos, avatar);
        }
        onQmlPlayersModelReset: {
            playersModel.reset()
        }
        onQmlCardsReset: {
            match.reset()
        }
        onQmlMatchStart:{
            match.state = "started"
        }
    }
}
