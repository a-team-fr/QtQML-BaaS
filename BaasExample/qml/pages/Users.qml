import QtQuick 2.11
import QtQuick.Controls 2.4
import BaaS.Example.Assets 1.0
import QtQuick.Layouts 1.3
import BaaS 1.0
import FontAwesome 1.0
import "../component"

Flickable {
    id:root
    anchors.fill:parent
    anchors.margins : 10
    contentHeight: content.childrenRect.height
    contentWidth : Math.max(width, content.childrenRect.width)


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
        id:content
        width : root.width



        GroupBox{
            title:qsTr("Users list")
            //Layout.alignment : Qt.AlignHCenter
            ATTableView{
                id:lstUsers
                model: baasmodel
                roles: baasmodel.rolesList
                showRemove : Assets.backend.signedIn
                testMode: false
                onRemove: Assets.backend.deleteUser(baasmodel.get(row,"objectId"))
            }
        }

        GroupBox{
            title:qsTr( "e-mail adapter" )
            width : root.width
            ColumnLayout{
                anchors.fill : parent
                RowLayout{
                    Layout.fillWidth: true
                    TextField{
                        id:email
                        placeholderText: qsTr("fill in email to check")

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
