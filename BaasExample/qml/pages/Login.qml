import QtQuick 2.11
import QtQuick.Controls 2.4
import BaaS.Example.Assets 1.0
import QtQuick.Layouts 1.3
import FontAwesome 1.0
import "../component"


Flickable {
    anchors.fill:parent
    contentHeight: content.childrenRect.height
    contentWidth : width

    Column{
        id:content
        anchors.fill:parent
        anchors.margins: 5

        TextField{
            id:login
            width : parent.width
            placeholderText: qsTr("Fill in your login")
        }
        TextField{
            id:password
            width : parent.width
            echoMode: TextInput.Password
            placeholderText: qsTr("Fill in your password")
        }

        RowLayout{
            width : parent.width
            AwesomeButton{
                enabled : Assets.backend.signedIn ? true : (login.text!="" && password.text !="")
                iconTxt: Assets.backend.signedIn ? FontAwesome.signOut : FontAwesome.signIn
                text : Assets.backend.signedIn ? qsTr("Sign out") : qsTr("Sign in")
                onClicked: Assets.backend.signedIn ? Assets.backend.signOut() : Assets.backend.signIn(login.text, password.text)
                color: "lightgrey"
            }


            LayoutSpring{ fillHeight :false}

            AwesomeButton{
                Layout.alignment: Qt.AlignRight
                enabled :  (login.text!="" && password.text !="")
                iconTxt: FontAwesome.plus
                baseColor: "green"
                text: qsTr("register")
                onClicked: Assets.backend.registerUser(login.text, password.text)
                color: "lightgreen"
            }

        }


        ATText{
            visible : text != ""
            color: "red"
            width:parent.width
            height:50
            text:Assets.backend.errorMessage
        }



        //LayoutSpring{}


    }
}


