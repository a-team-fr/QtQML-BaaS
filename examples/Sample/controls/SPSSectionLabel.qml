import QtQuick 2.0

Item{
    property string text : "Section title"
    property color color : "lightgrey"

    Text{
        text: parent.text
        color : parent.color
        font.pixelSize: 32*root.scaleFactor
        minimumPixelSize: 8
        fontSizeMode : Text.Fit
        width: parent.width
        height: parent.height
        verticalAlignment : Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
    }

}
