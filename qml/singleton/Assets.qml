import QtQuick 2.0
import BaaS 1.0
import Qt.labs.settings 1.0
//import FontAwesome 1.0

pragma Singleton

Item {
    property var saved : Settings {
        id:saved
        property alias hostURI: backend.hostURI
        property alias applicationId: backend.applicationId
        property alias masterKey: backend.masterKey
        property alias restKey: backend.restKey
        property alias extraHostURI : backend.extraHostURI
    }

    property var ui:QtObject{
        id:ui
        readonly property int width : 300
        readonly property int height : 500

        readonly property int maximumPointSize : 72
        readonly property int minimumPointSize : 3


    }

    property var nav:QtObject{
        id:nav
        property var lstPages:ListModel{
            id:lstPages
            ListElement{
                label:qsTr("About")
                url:"qrc:/qml/pages/Home.qml"
                iconTxt:"\uf015"//FontAwesome.home
            }
            ListElement{
                label:qsTr("Settings")
                url:"qrc:/qml/pages/Settings.qml"
                iconTxt:"\uf2bc"//FontAwesome.vcardO
            }
            ListElement{
                label:qsTr("Authentification")
                url:"qrc:/qml/pages/Login.qml"
                iconTxt:"\uf2be"//FontAwesome.userCircleO

            }
            ListElement{
                label:qsTr("Mon profile")
                url:"qrc:/qml/pages/Me.qml"
                iconTxt:"\uf2ba"//FontAwesome.addressBookO
            }
            ListElement{
                label:qsTr("Users")
                url:"qrc:/qml/pages/Users.qml"
                iconTxt:"\uf0c0"//FontAwesome.users
            }
            ListElement{
                label:qsTr("Files")
                url:"qrc:/qml/pages/FileHandling.qml"
                iconTxt:"\uf1c5"//FontAwesome.filePictureO
            }
            ListElement{
                label:qsTr("Objects")
                url:"qrc:/qml/pages/Objects.qml"
                iconTxt:"\uf02d"//FontAwesome.book
            }
        }
        property int curPageIndex : 0
        property var curPage : lstPages.get( curPageIndex )
        readonly property string homePage : "qrc:/qml/pages/Home.qml"

        function selectPage(pageIndex)
        {
            curPageIndex = pageIndex;
        }

    }

    property var backend : Parse{
        id:backend
        //Default values to have a quickly working configuration for evaluation
        //please use your own parse server instance credentials
        hostURI: "https://parseapi.back4app.com"
        applicationId: "Mom8WK3Pu2cNh2csSQEfi9IxQhs8qBIDVYWQuP46"
        restKey: "aA12Y45mnzuG8hIHFqlns7XUS0y8GFGPsUtznc9V"
    }











}




