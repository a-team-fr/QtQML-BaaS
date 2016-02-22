import QtQuick 2.0
//import QtGraphicalEffects 1.0

Item{
    property alias text : embText.text
    property alias imgSource : img.source
    signal clicked()
    property bool imgLeft: true
    property real imageRatio : 1
    property alias fontFamily : embText.font.family
    opacity : enabled ? 1 : 0.5

    Image{
        id:img
        height: Math.min(parent.width *0.5, parent.height*0.5)
        width: height * parent.imageRatio
        anchors.verticalCenter: parent.verticalCenter
        anchors.left : parent.imgLeft ? parent.left : embText.right
        source:"qrc:/images/buttonBackground.png"
    }

    Text
    {
        id: embText
        enabled: parent.enabled
        height: parent.height
        width: parent.width *0.5
        anchors.verticalCenter: parent.verticalCenter
        anchors.left : parent.imgLeft ? img.right : parent.left
        anchors.margins: globals.ui.buttonMargin
        color: globals.ui.buttonTextcolor
        font.family: globals.fonts.arista2light.name
        font.pixelSize: 32//globals.ui.textGodzilla
        minimumPixelSize: globals.ui.minimumPixelSize
        fontSizeMode : Text.Fit

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment : Text.AlignVCenter
    }


    MouseArea{
        anchors.fill:parent
        onClicked: parent.clicked()
        enabled: parent.enabled
    }

}





