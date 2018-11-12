import QtQuick 2.11
import QtQuick.Controls 2.4
import BaaS.Example.Assets 1.0
import FontAwesome 1.0
import "./component"

ToolBar{
    id:root
    height : heightValues[heightProfile]
    width : ApplicationWindow.window.width

    readonly property var heightValues :[ 30 , 100 , 200, 300, (ApplicationWindow.window.height - ApplicationWindow.header.height)]
    property int heightProfile : 0

    Behavior on height {
        SmoothedAnimation {
            velocity : 200
        }
    }



    Column{
        anchors.fill : parent

        Row{
            height: 25
            width: parent.width
            spacing : 3
            AwesomeButton{
                height : parent.height
                width:height
                iconTxt: FontAwesome.caretUp
                onClicked: root.heightProfile = Math.min( root.heightProfile+1, root.heightValues.length-1)
            }

            AwesomeButton{
                height : parent.height
                width:height
                iconTxt: FontAwesome.caretDown
                onClicked: root.heightProfile = Math.max( root.heightProfile-1, 0)
            }
            AwesomeButton{
                height : parent.height
                width:height
                visible : Assets.backend.masterKey !== ""
                iconTxt: Assets.backend.useMaster ? FontAwesome.unlock : FontAwesome.lock
                color: Assets.backend.useMaster ? "red" : "green"
                onClicked: Assets.backend.useMaster = !Assets.backend.useMaster
            }

            ATText{
                width:parent.width-75
                height : parent.height
                text : Assets.backend.userName ? Assets.backend.userName : qsTr("Not connected")
                horizontalAlignment: Text.AlignLeft
                opacity: Assets.backend.userName ? 1 : 0.7

                MouseArea{
                    anchors.fill:parent
                    onClicked:Assets.backend.signIn("charby","Krutagor44") //TODO : only for easing dev : to be removed!!!!
                    //onClicked: Assets.nav.selectPage(2) //go to auth
                }
            }

        }
        ProgressBar{
            height: 5
            width: parent.width
            from: 0
            to: 100
            value : Assets.backend.percComplete
            //visible : Assets.backend.percComplete !== 100
        }

        ListView{
            id:lstView
            height: root.height - 30
            width: parent.width
            clip : true
            highlightFollowsCurrentItem: true
            highlightRangeMode: ListView.StrictlyEnforceRange
            snapMode : ListView.SnapOneItem


            contentWidth: width
            contentHeight: count * 70

            model:ListModel{}
            delegate:Row{
                width:root.width
                height :root.height
                Rectangle{
                    width: 20
                    height :parent.height
                    color:"white"
                    ATText{
                        anchors.fill: parent
                        text: httpCode +"\n"+error
                        color: error ? "red" : "green"
                    }
                }
                ATText{
                    width: parent.width - 20
                    height :parent.height
                    text : httpResponse
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignTop
                    font.pointSize: 15
                    minimumPointSize: 2
                    wrapMode: Text.WordWrap
                }

            }


            ScrollIndicator.vertical: ScrollIndicator {}
        }

        Connections{
            target:Assets.backend
            onFinished:{
                if (Assets.backend.httpCode === -1) return;

                var jsObject = {
                    "httpCode":Assets.backend.httpCode,
                    "httpResponse":Assets.backend.httpResponseText,
                    "errorTxt":Assets.backend.namErrorText,
                    "error":Assets.backend.namError
                }
                //console.log("http response :"+Assets.backend.httpResponseText)
                lstView.model.append( jsObject)
                lstView.positionViewAtEnd()
                //console.log("new response:"+ Assets.backend.httpCode + " |resp:" + Assets.backend.httpResponse + " |error :" + Assets.backend.error  );
            }
        }




    }

    Rectangle{
        anchors.fill: parent
        color:"red"
        visible : !Assets.backend.ready
        ATText{
            anchors.fill: parent
            text: qsTr("Backend is not configured - fill in credentials in the settings")
            color: "white"
            wrapMode: Text.Wrap
        }
    }

}
