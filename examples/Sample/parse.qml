import QtQuick 2.3
import QtQuick.Controls 1.4
import BaaS 1.0
import "content"
import "controls"
import Qt.labs.settings 1.0

ApplicationWindow {
    id:root
    visible: true
    height:480
    width: 320
    property real scaleFactor: Math.min( height, width) / 320
    color: "#212126"

    Parse{
        id:backend
        //hostURI:"<PUT HERE YOUR SETTINGS"
        //applicationId:"PUT HERE YOUR SETTINGS"
        //masterKey:"PUT HERE YOUR SETTINGS"
    }

    Settings {
        property alias hostURI: backend.hostURI
        property alias applicationId: backend.applicationId
        property alias masterKey: backend.masterKey
    }

    toolBar: BorderImage {
        border.bottom: 8
        source: "images/toolbar.png"
        width: parent.width
        height: 50 * root.scaleFactor

        Rectangle {
            id: backButton
            width: opacity ? 60 : 0
            anchors.left: parent.left
            anchors.leftMargin: 2 * root.scaleFactor
            opacity: stackView.depth > 1 ? 1 : 0
            anchors.verticalCenter: parent.verticalCenter
            antialiasing: true
            height: 30 * root.scaleFactor
            radius: 4
            color: backmouse.pressed ? "#222" : "transparent"
            Behavior on opacity { NumberAnimation{} }
            Image {
                anchors.verticalCenter: parent.verticalCenter
                source: "images/navigation_previous_item.png"
            }
            MouseArea {
                id: backmouse
                anchors.fill: parent
                anchors.margins: -10
                onClicked: stackView.pop()
            }
        }

        SPSText {
            width : parent.width - backButton.width
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            color: "white"
            text: ((stackView.depth > 1) && stackView.currentItem) ? pageModel.get(stackView.currentItem.index).title : "BaaS Sample (using Parse server)"
        }
    }

    Rectangle{
        id:errorMessage
        width: parent.width*0.8; height: parent.height*0.8
        radius : 5
        z:2
        opacity : 0.8

        color:"red"
        visible:false
        property string errorText : backend.error
        onErrorTextChanged: {
            if (errorText != "") visible = true;
        }
        TextArea{
            textColor: "red"
            anchors.centerIn: parent
            text : backend.error
        }
        MouseArea{
            anchors.fill: parent
            onClicked: parent.visible = false
        }

    }

    statusBar: Rectangle{
        id:statusBar
        color:"black"
        height: 50 * root.scaleFactor
        width: parent.width


        Item{

            id: status
            height:parent.height
            x:5
            width : parent.width * 0.5

            SPSText{
                visible : backend.ready
                text: "Ready!" + "("+ backend.userName +")"
                horizontalAlignment: Text.AlignLeft
                color: "green"
                anchors.fill: parent
            }

            SPSProgressBar{
                visible: !backend.ready
                anchors.fill: parent
                minValue: 0
                maxValue: 100
                value : backend.percComplete
            }
        }
        SPSText{

            height:parent.height
            width: parent.width * 0.5
            anchors.right: parent.right
            visible : backend.ready
            text: "HTTP code : "+ backend.httpCode
            color: ((backend.httpCode>= 200)&&(backend.httpCode< 300)) ? "green" : "red"
        }
        Rectangle{
            anchors.fill: parent
            color:"red"
            visible : !backend.ready && backend.percComplete==100
            SPSText{
                width: parent.width; height: parent.height
                text: "Backend is not configured - fill in credentials in the settings"
                color: "black"
                wrapMode: Text.Wrap
            }
        }

    }

    ListModel {
        id: pageModel
        ListElement {
            title: "Settings"
            page: "content/PageSettings.qml"
        }
        ListElement {
            title: "Users"
            page: "content/PageUsers.qml"
        }

    }

    StackView {
        id: stackView
        anchors.fill: parent
        // Implements back key navigation
        focus: true
        Keys.onReleased: if (event.key === Qt.Key_Back && stackView.depth > 1) {
                             stackView.pop();
                             event.accepted = true;
                         }

        initialItem: Item {
            width: parent.width
            height: parent.height
            ListView {
                model: pageModel
                anchors.fill: parent
                delegate: DelegateMenuPage {
                    text: title
                    onClicked: stackView.push(Qt.resolvedUrl(page))
                }
            }
        }
    }

}
