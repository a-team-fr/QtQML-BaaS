import QtQuick 2.11
import QtQuick.Controls 2.4
import FontAwesome 1.0
import QtGraphicalEffects 1.0

Rectangle{
    id:root
    property alias iconTxt :icon.text
    property alias iconElem : icon
    signal clicked();

    property bool draggable : false

    height : 56
    width : 56
    radius : width / 2
    color : "transparent"

    layer.enabled: !ma.pressed
    layer.effect: DropShadow {
        verticalOffset: 2
        horizontalOffset: 1
        color: "grey"
        samples: 20
        spread: 0.5
    }

    Text{
        id : icon
        anchors.fill: parent
        anchors.margins : 5
        color : "white"
        font.pixelSize: 24
        font.family: FontAwesome.fontFamily
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment : Text.AlignVCenter
    }
    MouseArea{
        id: ma
        hoverEnabled:true
        anchors.fill : parent
        onClicked: root.clicked();
        drag.target : root.draggable ? root : null
        drag.axis : Drag.XAndYAxis
        drag.minimumX: 0
        drag.minimumY: 0
        drag.maximumX: root.parent.width - root.width
        drag.maximumY: root.parent.height - root.height

    }

}


