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
        endPoint: backend.endPoint
    }
    Connections{
        target: backend
        onCurrentObjectChanged:{
            result.text += "<p style='color:blue'><b>Object changed:</b>"+JSON.stringify(currentObject, null, '') + "</p>";
            baasmodel.resetModel();
        }
        onObjectDeleted:{
            result.text += "<p style='color:orange'>Object deleted:" + deletedObjectId +"</p>"
            //remove deleted object suffix from endpoint
            var str = backend.endPoint;
            backend.endPoint = str.substring( 0, str.length - deletedObjectId.length - 1)

        }

        onReplyFinished:{
            result.text += "<p style='color:green'><b>replyFinished:</b>"+JSON.stringify(doc, null, '<br>') + "</p>";
        }

        //onEndPointChanged: baasmodel.endPoint = backend.endPoint
        onQueryFailed:{
            result.text += "<p style='color:red'><b>Query failed:</b>"+JSON.stringify(doc, null, '<br>') + "</p>";
        }
    }

    Flickable{
       id:page
       width: parent.width; height: parent.height
       contentWidth: width; contentHeight: 30 * 30 //contentItem.childrenRect.height

       Column{
           width: parent.width - 2*marginWidth;
           property int marginWidth : 30
           x: marginWidth; y:marginWidth
           property int itemHeight : 30
           spacing: 2

           SPSSectionLabel{
               text:"EndPoint :"
               x:- 0.5 * parent.marginWidth
               width:parent.width
               height : parent.itemHeight * 2
           }
           Row{
               width:parent.width
               height : parent.itemHeight
               SPSTextField{
                   id: endPoint
                   text:backend.endPoint
                   width:parent.width - height
                   height : parent.height
                   placeholderText: "current backend endpoint"
                   focus:true
                   //validator: RegExpValidator{ regExp: /classes\/.*/}
                   onEditingFinished: {
                       var EPTmp = text;
                       if (EPTmp.charAt(EPTmp.length-1) == "/")
                           EPTmp = EPTmp.substring(0, EPTmp.length -1)
                       if ( EPTmp.substring(0, 7) != "classes")
                           EPTmp = "classes/" + EPTmp;
                       backend.endPoint = EPTmp;
                       //baasmodel.endPoint = EPTmp;

                       //backend.ensureEndPointHasPrefix("classes/")
                   }
               }
               SPSButtonText{
                   height : parent.height
                   width:height
                   text:"<"
                   onClicked: {
                       var EPTmp = endPoint.text;
                       var ind = EPTmp.lastIndexOf("/");
                       if (ind >0)
                           backend.endPoint = EPTmp.substring(0, ind)
                   }
               }
           }

           SPSSectionLabel{
               text:"Request :"
               x:- 0.5 * parent.marginWidth
               width:parent.width
               height : parent.itemHeight * 2
           }

           SPSJSonTextEdit{
               id:reqContent
               width:parent.width
               height : parent.itemHeight * 5
           }

           Row{
               width:parent.width
               height : parent.itemHeight
               SPSButtonText{
                   width:parent.width * .25
                   height : parent.height
                   text:"Create"
                   onClicked: backend.create( reqContent.text)
               }
               SPSButtonText{
                   width:parent.width * .25
                   height : parent.height
                   text:"Update"
                   onClicked: backend.update(reqContent.text)
               }
               SPSButtonText{
                   width:parent.width * .25
                   height : parent.height
                   text:"Get"
                   onClicked: backend.get()
               }
               SPSButtonText{
                   width:parent.width * .25
                   height : parent.height
                   text:"Delete"
                   onClicked: backend.deleteObject(reqContent.text)
               }
           }



           SPSSectionLabel{
               text:"Result :"
               x:- 0.5 * parent.marginWidth
               width:parent.width
               height : parent.itemHeight * 2
           }

           SPSJSonTextEdit{
               id:result
               width:parent.width
               height : parent.itemHeight * 5
               readOnly: true
           }
           SPSButtonText{
               width:100
               height : 20
               text:"Refresh"
               onClicked: baasmodel.resetModel()
           }

           BaaSListView{
               model: baasmodel
               roles: baasmodel.rolesList
               visibleRows: 4
               width:parent.width
               height : parent.itemHeight * 5
               showRemove : false
               onRowClicked: {
                   backend.endPoint = backend.endPoint + "/" + baasmodel.get(row,"objectId");
               }
           }

       }
    }

    /*Component.onCompleted: {
        backend.endPoint = "classes/"
    }*/
}
