import QtQuick 2.0

Item
{
    id:mySwitch
    property color bgColor : "blue"
    property alias color : ball.color
    property bool switchOn : false

    signal switched( bool switchOn)
    Rectangle{
        id:background
        anchors.fill: parent
        color : parent.switchOn ? Qt.darker(parent.bgColor) : parent.bgColor
        border.color: "grey"
        border.width: 2
        radius : height*0.5

        Rectangle{
            id:ball
            height: parent.height - 2 * parent.border.width
            width : height
            radius : height * 0.5
            anchors.verticalCenter: parent.verticalCenter
            x : mySwitch.switchOn ? parent.width - width : 0
            color:"white"
        }
    }
    MouseArea{
        anchors.fill: parent
        onClicked: {

            mySwitch.switchOn = !mySwitch.switchOn
            mySwitch.switched(mySwitch.switchOn);
        }
    }

}
