import QtQuick 2.11
import QtQuick.Controls 2.4
import BaaS.Example.Assets 1.0
import QtQuick.Layouts 1.3
import "../component"


Flickable {
    contentHeight: content.childrenRect.height
    contentWidth : width
    ColumnLayout{
        id:content
        width:parent.width * 0.9
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 5

        GroupBox{
            title:"Parse server configuration"
            width:parent.width
            Layout.alignment: Qt.AlignHCenter

            ColumnLayout{
                anchors.fill : parent
                anchors.margins: 5
                TextField{
                    text: Assets.backend.hostURI
                    Layout.fillWidth: true
                    placeholderText: qsTr("Fill in your backend host url")
                    onEditingFinished: Assets.backend.hostURI = text
                }
                TextField{
                    text: Assets.backend.applicationId
                    Layout.fillWidth: true
                    placeholderText: qsTr("Fill in your application ID")
                    onEditingFinished: Assets.backend.applicationId = text
                }
                TextField{
                    text: Assets.backend.masterKey
                    Layout.fillWidth: true
                    echoMode: TextInput.Password
                    placeholderText: qsTr("Fill in your Master key (keep it private)")
                    onEditingFinished: Assets.backend.masterKey = text
                }
                TextField{
                    text: Assets.backend.restKey
                    Layout.fillWidth: true
                    echoMode: TextInput.Password
                    placeholderText: qsTr("Fill in your REST key")
                    onEditingFinished: Assets.backend.restKey = text
                }
            }
        }


        LayoutSpring{}


    }
}

