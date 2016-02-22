import QtQuick 2.0
import QtGraphicalEffects 1.0

Item{
    property alias text : embText.text
    property alias textColor : embText.color
    property color color : "white"
    signal clicked()
    property alias fontFamily: embText.font.family
    
    Rectangle{
        id:embRect
        border.color: "white"
        border.width: 2
        radius: height * 0.5
        opacity: 0.3

        width:parent.height
        height:parent.width
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        transformOrigin: Item.Center
        rotation:-90

        gradient: Gradient{
            GradientStop { position: 0.0; color: "white"}//#A07E8F" }
            GradientStop { position: 1.0; color: "black"}//"#6E4F61" }
        }

    }

    SPSText
    {
        id: embText
        enabled: parent.enabled
        anchors.fill: parent
        anchors.margins: 5
        color: "white"

    }

    MouseArea{
        id:embMA
        anchors.fill:parent
        enabled: parent.enabled
        onClicked: parent.clicked()
    }

}



