import QtQuick 2.5
import QtQuick.Controls 1.4

import "../controls"

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



    Flickable{
       id:page
       width: parent.width; height: parent.height
       contentWidth: width; contentHeight: contentPane.height


       Column{
           id:contentPane
           width: parent.width - 10;

           x: 5; y:5
           property int itemHeight : 30
           SPSText{
               text:"Backend server base URL :"
               width:parent.width
               height : parent.itemHeight
               horizontalAlignment: Text.AlignLeft
           }
           SPSTextField{
               id:backendhost
               width:parent.width
               height : parent.itemHeight
               text: backend.hostURI
               placeholderText: "Fill in your backend host url"
               onEditingFinished: backend.hostURI = text
           }
           SPSText{
               text:"Application id :"
               width:parent.width
               height : parent.itemHeight
               horizontalAlignment: Text.AlignLeft
           }
           SPSTextField{
               id:applicationId
               width:parent.width
               height : parent.itemHeight
               text: backend.applicationId
               placeholderText: "Fill in your application ID"
               onEditingFinished: backend.applicationId = text
           }
           SPSText{
               text:"Master key :"
               width:parent.width
               height : parent.itemHeight
               horizontalAlignment: Text.AlignLeft
           }
           SPSTextField{
               id:masterKey
               width:parent.width
               height : parent.itemHeight
               text: backend.masterKey
               echoMode: TextInput.Password
               placeholderText: "Fill in your Master key (keep it private)"
               onEditingFinished: backend.masterKey = text
           }


       }
    }
}
