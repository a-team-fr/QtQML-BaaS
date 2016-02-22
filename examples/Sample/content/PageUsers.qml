import QtQuick 2.5
import QtQuick.Controls 1.4
import "../controls"
import BaaS 1.0

Item {

    width: parent.width
    height: parent.height

    property real progress: 0
    SequentialAnimation on progress {
        loops: Animation.Infinite
        running: true
        NumberAnimation {
            from: 0
            to: 1
            duration: 3000
        }
        NumberAnimation {
            from: 1
            to: 0
            duration: 3000
        }
    }

    BaaSModel{
        id: baasmodel
        client:backend
        endPoint:"users"
    }

    Flickable{
       id:page
       width: parent.width; height: parent.height
       contentWidth: width; contentHeight: contentPane.height


       Column{
           id:contentPane
           width: parent.width - 10;

           x: 5; y:5
           property int itemHeight : 30
           spacing: 2
           SPSText{
               text:"Register :"
               width:parent.width
               height : parent.itemHeight
               horizontalAlignment: Text.AlignLeft
           }
           SPSTextField{
               id: userName
               width:parent.width
               height : parent.itemHeight
               placeholderText: "user name"
           }
           SPSTextField{
               id: password
               width:parent.width
               height : parent.itemHeight
               echoMode : TextInput.Password
               placeholderText: "password"
           }
           Row{
               width:parent.width
               height : parent.itemHeight
               SPSTextField{
                   id:email
                   width:parent.width * .7
                   height : parent.height
                   placeholderText:"e-mail"

               }
               SPSButtonText{
                   width:parent.width * .3
                   height : parent.height
                   text: "forget?"
                   onClicked: backend.passwordReset(email.text)

               }
           }
           Row{
               width:parent.width
               height : parent.itemHeight
               SPSButtonText{
                   width:parent.width * .5
                   height : parent.height
                   text:"Sign up"
                   onClicked: backend.signup(userName.text, password.text)
               }
               SPSButtonText{
                   width:parent.width * .5
                   height : parent.height
                   text: backend.loggedIn ? "Log out" : "Log in"
                   onClicked: {
                       if (backend.loggedIn)
                           backend.logout();
                       else
                            backend.login(userName.text, password.text)
                   }
               }
           }


           ListView{
               id:lstView
               width:parent.width
               height : parent.itemHeight * 10
               model:baasmodel
               header:Row{
                   Text{ text:  "UserName"}
                   Text{ text:  "email"}
                   Text{ text:  "createdAt"}
                   Text{ text:  "updatedAt"}
                   Text{ text:  "objectId"}
               }


               delegate:Row{
                   width : lstView.width
                   height : contentPane.itemHeight
                   Text{ text:  model.UserName}
                   Text{ text:  model.email}
                   Text{ text:  model.createdAt}
                   Text{ text:  model.updatedAt}
                   Text{ text:  model.objectId}

               }
           }



       }
    }
}
