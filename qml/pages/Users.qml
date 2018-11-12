import QtQuick 2.11
import QtQuick.Controls 2.4
import BaaS.Example.Assets 1.0
import QtQuick.Layouts 1.3
import BaaS 1.0
import FontAwesome 1.0
import "../component"

Pane {
    id:root
    anchors.fill:parent
    anchors.margins : 10


    BaaSModel{
        id: baasmodel
        source: Assets.backend
        endPoint:"users"
        prefix : "users"
    }

    Connections{
        target:Assets.backend
        onMasterModeChanged:baasmodel.reload()
    }

    ColumnLayout
    {
        anchors.fill: parent
        spacing : 5

        GroupBox{
            title:qsTr("Users list")
            Layout.fillWidth: true
            Layout.fillHeight: true
            Flickable{
                anchors.fill: parent
                contentWidth: lstUsers.width
                contentHeight: lstUsers.height
                clip : true
                ATTableView{
                    id:lstUsers
                    model: baasmodel
                    roles: baasmodel.rolesList
                    showRemove : Assets.backend.signedIn
                    testMode: false
                    onRemove: Assets.backend.deleteUser(baasmodel.get(row,"objectId"))
                }
            }


        }

        GroupBox{
            title:qsTr( "e-mail adapter" )
            width : root.width
            Layout.fillWidth: true
            //Layout.fillHeight: true
            ColumnLayout{
                anchors.fill : parent
                RowLayout{
                    Layout.fillWidth: true
                    TextField{
                        id:email
                        placeholderText: qsTr("fill in email to check")
                        Layout.fillWidth: true
                        inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhEmailCharactersOnly
                    }
                    Button{
                        text:"verify email"
                        enabled : email.text !== ""
                        onClicked : Assets.backend.verifyEmail( email.text)
                    }
                    Button{
                        text:"reset password"
                        enabled : email.text !== ""
                        onClicked : Assets.backend.passwordReset( email.text)
                    }
                }

            }
        }


    }






}
