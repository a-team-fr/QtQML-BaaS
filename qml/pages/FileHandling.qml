import QtQuick 2.11
import QtQuick.Controls 2.4
import BaaS.Example.Assets 1.0
import QtQuick.Layouts 1.3
import FontAwesome 1.0
import QtQuick.Dialogs 1.2
//import Qt.labs.platform 1.0
import BaaS 1.0
import "../component"

Pane {
    anchors.fill:parent
    anchors.margins : 10


    BaaSModel{
        id: baasmodel
        source:Assets.backend
        endPoint: "classes/FilesList"
    }


    ColumnLayout{
        id:content
        spacing: 5
        anchors.fill : parent

        GroupBox{
            title:qsTr("Upload file ")

            implicitWidth:parent.width//Layout.preferredWidth: parent.width//Layout.fillWidth:true//width:parent.width
            ColumnLayout{
                implicitWidth:parent.width//Layout.preferredWidth: parent.width//Layout.fillWidth:true//width:parent.width
                RowLayout{
                    implicitWidth:parent.width//Layout.preferredWidth: parent.width//Layout.fillWidth:true//width:parent.width
                    TextField{
                        id: filePath
                        Layout.fillWidth: true
                        placeholderText: "file path"
                        enabled: false
                    }
                    Button{
                        text:"..."
                        onClicked: fileDialog.visible=true
                    }
                }
                RowLayout{
                    Button{
                        text:"Upload"
                        onClicked: Assets.backend.uploadFile( filePath.text )
                    }
                    AwesomeButton{
                        Layout.alignment:Qt.AlignRight
                        text : qsTr("refresh list")
                        iconTxt: FontAwesome.refresh
                        onClicked: baasmodel.reload()
                    }

                }

            }
        }

        GroupBox{
            title:qsTr("File list - click on a row to display as image")
            implicitWidth:parent.width
            Layout.fillHeight:true
            ColumnLayout{
                anchors.fill:parent
                Flickable{
                    implicitWidth:parent.width
                    Layout.fillHeight:true
                    contentWidth: lstFile.width
                    contentHeight: lstFile.height
                    clip : true
                    ATTableView{
                        id:lstFile
                        model: baasmodel
                        roles: baasmodel.rolesList
                        showRemove : true
                        onRowClicked:
                        {
                            var file = baasmodel.get(row,"file");
                            console.log(file)
                            console.log(file.url)
                            showImage.source = file.url
                        }
                        onRemove: {
                            //remove file
                            Assets.backend.deleteFile( baasmodel.get(row,"name") );

                            //remove associated object (once backend is ready)
                            Assets.backend.objectDeleted.connect( function(){
                                Assets.backend.endPoint="classes/FileList";
                                Assets.backend.deleteObject( baasmodel.get(row,"objectId") );
                            });
                        }
                    }
                }


                Image{
                    id:showImage
                    Layout.fillHeight:true
                    width:parent.width
                    fillMode:Image.PreserveAspectFit

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
        //folder: shortcuts.home
        onAccepted: filePath.text = fileDialog.fileUrl
        onRejected: visible=false

    }


}
