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
       contentWidth: width; contentHeight: contentItem.childrenRect.height

       Column{
           width: parent.width - 2*marginWidth;
           property int marginWidth : 30
           x: marginWidth; y:marginWidth
           property int itemHeight : 30
           spacing: 2

           SPSSectionLabel{
               text:"Register :"
               x:- 0.5 * parent.marginWidth
               width:parent.width
               height : parent.itemHeight * 2
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

           Row{
               width:parent.width
               height : parent.itemHeight
               visible: false //This is not working yet
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

           SPSSectionLabel{
               text:"Users list :"
               x:- 0.5 * parent.marginWidth
               width:parent.width
               height : parent.itemHeight * 2
           }
           BaaSListView{
               model: baasmodel
               roles: baasmodel.rolesList
               visibleRows: 4
               width:parent.width
               height : parent.itemHeight * 5
               showRemove : backend.loggedIn

               onRemove: backend.deleteUser(baasmodel.get(row,"objectId"))
           }






       }
    }
}
