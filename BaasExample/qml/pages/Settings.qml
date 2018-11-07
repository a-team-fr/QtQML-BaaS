import QtQuick 2.11
import QtQuick.Controls 2.4
import BaaS.Example.Assets 1.0
import QtQuick.Layouts 1.3
import "../component"


Flickable {
    anchors.fill: parent
    contentWidth : width
    contentHeight: content.childrenRect.height
    Column{
        id:content
        width:parent.width * 0.9
        anchors.horizontalCenter: parent.horizontalCenter

        GroupBox{
            title:"Parse server configuration"
            width:parent.width
            Layout.alignment: Qt.AlignHCenter

            Column{
                anchors.fill : parent
                anchors.margins: 5
                spacing : 5
                TextField{
                    text: Assets.backend.hostURI
                    width:parent.width
                    placeholderText: qsTr("Fill in your backend host url")
                    onEditingFinished: Assets.backend.hostURI = text
                    ToolTip.text : qsTr("Backend host url")
                    ToolTip.visible:hovered
                }
                TextField{
                    text: Assets.backend.applicationId
                    width:parent.width
                    placeholderText: qsTr("Fill in your application ID")
                    onEditingFinished: Assets.backend.applicationId = text
                    ToolTip.text : qsTr("Application Id")
                    ToolTip.visible:hovered
                }
                TextField{
                    text: Assets.backend.masterKey
                    width:parent.width
                    echoMode: TextInput.Password
                    placeholderText: qsTr("Fill in your Master key (keep it private)")
                    onEditingFinished: Assets.backend.masterKey = text
                    ToolTip.text : qsTr("Master key")
                    ToolTip.visible:hovered
                }
                TextField{
                    text: Assets.backend.restKey
                    width:parent.width
                    echoMode: TextInput.Password
                    placeholderText: qsTr("Fill in your REST key")
                    onEditingFinished: Assets.backend.restKey = text
                    ToolTip.text : qsTr("Rest key")
                    ToolTip.visible:hovered
                }
            }
        }

    }
}

