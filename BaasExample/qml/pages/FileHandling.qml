import QtQuick 2.11
import QtQuick.Controls 2.4
import BaaS.Example.Assets 1.0
import QtQuick.Layouts 1.3
import FontAwesome 1.0
import QtQuick.Dialogs 1.2
//import Qt.labs.platform 1.0
import BaaS 1.0
import "../component"

Flickable {
    anchors.fill:parent
    contentHeight: content.childrenRect.height
    contentWidth : width

    BaaSModel{
        id: baasmodel
        source:Assets.backend
        endPoint: "classes/FilesList"
    }

    Connections{
        target: Assets.backend
        onFileUploaded:{
            //Associate with object
            var obj = {
                        "url": currentObject.url,
                        "file": {
                            "name": currentObject.name,
                            "__type": "File"
                            }
                        }
            Assets.backend.endPoint = "classes/FilesList"
            Assets.backend.createObject( JSON.stringify(obj));

        }
    }

    ColumnLayout{
       id:content
       width: parent.width
       spacing: 2

       GroupBox{
           width:parent.width
           title:qsTr("Upload file ")
           ColumnLayout{
               anchors.fill: parent
               anchors.margins : 5
               RowLayout{
                   width:parent.width
                   height : 30
                   TextField{
                       id: filePath
                       Layout.fillWidth: true
                       placeholderText: "file path"
                       enabled: false
                   }
                   Button{
                       width:20
                       height : parent.height
                       text:"..."
                       onClicked: fileDialog.visible=true
                   }
               }

               Button{
                   width:parent.width
                   height : 30
                   text:"Upload"
                   onClicked: Assets.backend.uploadFile( filePath.text )
               }

           }
       }

       GroupBox{
           title:qsTr("File list - click on a row to display as image")
           width:parent.width
           ColumnLayout{
               anchors.fill: parent
               anchors.margins : 5

               ATTableView{
                   model: baasmodel
                   roles: baasmodel.rolesList
                   width:parent.width
                   height : 50
                   showRemove : true
                   onRowClicked: showImage.source = baasmodel.get(row,"url")
                   onRemove: {
                       //remove file
                       Assets.backend.deleteFile( baasmodel.get(row,"file") );

                       //remove associated object (once backend is ready)
                       Assets.backend.objectDeleted.connect( function(){
                           Assets.backend.endPoint="classes/FileList";
                           Assets.backend.deleteObject( baasmodel.get(row,"objectId") );
                       });
                   }
               }

               AwesomeButton{
                   text : qsTr("refresh list")
                   iconTxt: FontAwesome.refresh
                   onClicked: baasmodel.reload()
                   height : 30

               }

               Image{
                   id:showImage
                   width : parent.width
                   height:50


               }

           }
       }






    }


    FileDialog {
        id: fileDialog
        width: parent.width
        height:parent.height
        visible:false
        selectExisting:true
        selectMultiple: false
        title: qsTr("Please choose a file")
        folder: shortcuts.home
        onAccepted: filePath.text = fileDialog.fileUrl
        onRejected: visible=false

    }


}
