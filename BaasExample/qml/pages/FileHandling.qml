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
    anchors.margins : 10
    contentHeight: content.childrenRect.height
    contentWidth : content.childrenRect.width

    BaaSModel{
        id: baasmodel
        source:Assets.backend
        endPoint: "classes/FilesList"
    }

    Connections{
        target: Assets.backend
        onFileUploaded:{
            //Associate with object
            var obj = {"name": currentObject.name,
                        "file": {
                            "name": currentObject.name,
                            "url": currentObject.url,
                            "__type": "File"
                            }
                        }
            Assets.backend.endPoint = "classes/FilesList"
            Assets.backend.createObject( JSON.stringify(obj));

        }
    }

    ColumnLayout{
       id:content
       spacing: 5

       GroupBox{
           Layout.fillWidth: true
           title:qsTr("Upload file ")
           ColumnLayout{
               Layout.fillWidth: true
               RowLayout{
                   Layout.fillWidth: true
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
           Layout.fillWidth: true
           ColumnLayout{

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
