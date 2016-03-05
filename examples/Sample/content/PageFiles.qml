import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
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
        endPoint: "classes/FilesList"
    }

    Connections{
        target: backend
        onCurrentObjectChanged:{
            result.text += "<p style='color:blue'><b>Object changed:</b>"+JSON.stringify(currentObject, null, '') + "</p>";
            baasmodel.resetModel();
        }
        onObjectDeleted:{
            result.text += "<p style='color:orange'>Object deleted:" + deletedObjectId +"</p>"
            baasmodel.resetModel();

        }

        onReplyFinished:{
            result.text += "<p style='color:green'><b>replyFinished:</b>"+JSON.stringify(doc, null, '<br>') + "</p>";
        }

        //onEndPointChanged: baasmodel.endPoint = backend.endPoint
        onQueryFailed:{
            result.text += "<p style='color:red'><b>Query failed:</b>"+JSON.stringify(doc, null, '<br>') + "</p>";
        }

        onFileUploaded:{
            result.text += "<p style='color:blue'><b>File uploaded:</b>"+JSON.stringify(currentObject, null, '<br>') + "</p>";
            //Associate with object
            var obj = {
                        "url": currentObject.url,
                        "file": {
                            "name": currentObject.name,
                            "__type": "File"
                            }
                        }
            backend.endPoint = "classes/FilesList"
            backend.create( JSON.stringify(obj));

        }
    }


    Flickable{
       id:page
       width: parent.width; height: parent.height
       contentWidth: width; contentHeight: 30 * 17 //contentItem.childrenRect.height

       Column{
           width: parent.width - 2*marginWidth;
           property int marginWidth : 30
           x: marginWidth; y:marginWidth
           property int itemHeight : 30
           spacing: 2

           SPSSectionLabel{
               text:"Upload file :"
               x:- 0.5 * parent.marginWidth
               width:parent.width
               height : parent.itemHeight * 2
           }

           Row{
               width:parent.width
               height : parent.itemHeight
               SPSTextField{
                   id: filePath
                   text:"file:///Users/charby/Pictures/img_0001.jpg"// "/Users/charby/Downloads/git_p2_activity.txt"
                   width:parent.width - 50
                   height : parent.height
                   placeholderText: "file path"
                   enabled: false


               }
               SPSButtonText{
                   width:50
                   height : parent.height
                   text:"..."
                   onClicked: fileDialog.visible=true
               }
           }

           SPSButtonText{
               width:parent.width
               height : parent.itemHeight
               text:"Upload"
               onClicked: backend.uploadFile(filePath.text)
           }


           SPSSectionLabel{
               text:"File list :"
               x:- 0.5 * parent.marginWidth
               width:parent.width
               height : parent.itemHeight * 2
           }

           SPSButtonText{
               width:100
               height : 20
               text:"Refresh"
               onClicked: baasmodel.resetModel()
           }

           SPSJSonTextEdit{
               id:result
               width:parent.width
               height : parent.itemHeight * 5
               readOnly: true
           }


           BaaSListView{
               model: baasmodel
               roles: baasmodel.rolesList
               visibleRows: 4
               width:parent.width
               height : parent.itemHeight * 5
               showRemove : true
               onRowClicked: Qt.openUrlExternally(baasmodel.get(row,"url"))
               onRemove: {
                   //remove file
                   backend.deleteFile( baasmodel.get(row,"file") );

                   //remove associated object (once backend is ready)
                   backend.objectDeleted.connect( function(){
                       backend.endPoint="classes/FileList";
                       backend.deleteObject( baasmodel.get(row,"objectId") );
                   });


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
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted: filePath.text = fileDialog.fileUrl
        onRejected: visible=false

    }

}
