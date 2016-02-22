import QtQuick 2.0

Item{
    id:root
    property color innerColor: "grey"
    property color outerColor: "white"
    property color borderColor: "lightgrey"
    property bool checked : false
    property alias text : embText.text

    Row{
        anchors.fill: parent
        spacing: 5
        Rectangle{
            anchors.verticalCenter: parent.verticalCenter
            color:root.outerColor
            height : embText.contentHeight * 1.5
            width : height
            radius : height *0.5
            border.width : 1
            border.color : root.outerColor
            Rectangle{
                anchors.centerIn: parent
                color: root.innerColor
                height : parent.height * 0.6
                width: height
                radius : height *0.5
                visible : root.checked
            }
        }

        SPSText{
            id:embText
            width : parent.width * 0.8
            height : parent.height
            horizontalAlignment: Text.AlignLeft
            anchors.verticalCenter: parent.verticalCenter
        }

    }
    MouseArea{
        anchors.fill : parent
        onClicked: {
            root.checked = !root.checked

        }
    }
}
