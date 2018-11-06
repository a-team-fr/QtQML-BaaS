import QtQuick 2.11
import QtQuick.Controls 2.4
import BaaS.Example.Assets 1.0
import QtQuick.Layouts 1.11
import FontAwesome 1.0
import "../component"


Flickable {
    anchors.fill:parent
    contentHeight: content.childrenRect.height
    contentWidth : width

    Label{
        id:content
        width:100
        height:100
    }

//    GridLayout{
//        id:content
//        width: parent.width

//        columns : 3
//        rows:3
//        rowSpacing : 1
//        columnSpacing :1

////        Rectangle{
////            width : content.width
////            height: 20
////            color:"grey"
////            //text:"your profile image"
////            Layout.row : 1
////            Layout.column :1

////            Layout.columnSpan : 3
////        }
////        Rectangle{
////            width : 100
////            height: 100
////            Layout.fillHeight: true
////            Layout.fillWidth: true
////            color:"blue"
////            Layout.row : 2
////            Layout.column :1
////            Layout.rowSpan: 2

////        }
////        Rectangle{
////            width : 100
////            height: 100
////            Layout.fillHeight: true
////            Layout.fillWidth: true
////            color:"green"
////            Layout.row : 2
////            Layout.column :2
////            Layout.rowSpan: 2
////            Layout.columnSpan: 2

////        }
////        Rectangle{
////            width : 100
////            height: 100
////            Layout.fillHeight: true
////            Layout.fillWidth: true
////            color:"orange"
////            Layout.row : 4
////            Layout.column :1

////        }
////        Rectangle{
////            width : 100
////            height: 100
////            Layout.fillHeight: true
////            Layout.fillWidth: true
////            color:"orange"
////            Layout.row : 4
////            Layout.column :2

////        }
////        Rectangle{
////            width : 100
////            height: 100
////            Layout.fillHeight: true
////            Layout.fillWidth: true
////            color:"orange"
////            Layout.row : 4
////            Layout.column :3

////        }
//    }
}



//        Rectangle{
//            Layout.fillHeight: true
//            Layout.fillWidth: true
//            color:"lightgrey"
//            //text:"your personnal data"
//            Layout.rowSpan : 3
//        }
//        Rectangle{
//            Layout.fillHeight: true
//            Layout.fillWidth: true
//            color:"lightgrey"
//            //text:"Username"
//            Layout.column :2
//            Layout.row : 4
//        }
//        Rectangle{
//            Layout.fillHeight: true
//            Layout.fillWidth: true
//            color:"lightgrey"
//            //text:"Charby"
//            Layout.column :3
//            Layout.row : 4
//        }
//        Rectangle{
//            Layout.fillHeight: true
//            Layout.fillWidth: true
//            color:"lightgrey"
//            //text:"e-mail"
//            Layout.column :2
//            Layout.row : 5
//        }
//        Rectangle{
//            Layout.fillHeight: true
//            Layout.fillWidth: true
//            color:"lightgrey"
//            //text:"truc@prout.fr"
//            Layout.column :3
//            Layout.row : 5
//        }
//        Rectangle{
//            Layout.fillHeight: true
//            Layout.fillWidth: true
//            color:"lightgrey"
//            //text:"Save info"
//        }


//    }
//    /*
//    FileDialog {
//        id: fileDialog
//        width: parent.width
//        height:parent.height
//        visible:false
//        selectExisting:true
//        selectMultiple: false
//        title: "Please choose a file"
//        folder: shortcuts.home
//        onAccepted: filePath.text = fileDialog.fileUrl
//        onRejected: visible=false

//    }*/
//}


