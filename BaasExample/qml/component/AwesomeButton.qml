import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import FontAwesome 1.0
import QtQuick.Controls.Material 2.1

Rectangle{
    id:root
    property alias iconTxt :icon.text
    property alias text : label.text
    property alias iconElem : icon
    property alias textElem : label
    property bool highlighted: false //TODO
    property color baseColor : Material.foreground// "black"
    readonly property color usedColor : highlighted ? "red" : baseColor
    signal clicked();

    property bool draggable : false

    Layout.preferredHeight : 20
    Layout.preferredWidth : 100
    color : "transparent"

    RowLayout {
        id:layout
        anchors.fill: parent

        Text{
            id : icon
            color : root.usedColor
            property bool isEmpty : text === ""
            font.family: FontAwesome.fontFamily
            Layout.preferredHeight: isEmpty ? 0 : root.height
            Layout.preferredWidth : height
            height: isEmpty ? 0 : root.height
            width : height

            visible : !isEmpty
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment : Text.AlignVCenter

            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
        }
        Label{
            id:label
            color : root.usedColor
            property bool isEmpty : text === ""
            horizontalAlignment: icon.isEmpty ? Text.AlignHCenter :  Text.AlignLeft
            verticalAlignment : Text.AlignVCenter
            //Layout.fillHeight: true
            //Layout.fillWidth: true
            Layout.preferredWidth : 200

//            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
        }

    }
    MouseArea{
        id:ma
        enabled : root.visible
        hoverEnabled:true
        anchors.fill : parent
        onClicked: root.clicked();
        //propagateComposedEvents: true
        drag.target : root.draggable ? root : null
        drag.axis : Drag.XAndYAxis
        drag.minimumX: 0
        drag.minimumY: 0
        drag.maximumX: root.parent.width - root.width
        drag.maximumY: root.parent.height - root.height

    }

}


