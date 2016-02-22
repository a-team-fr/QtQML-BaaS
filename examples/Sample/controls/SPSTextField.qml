import QtQuick 2.4

FocusScope {
    id:textField
    property alias text : embTextInput.text
    property alias placeholderText: placeholder.text
    property alias color : embTextInput.color
    property alias backgroundColor : embRect.color
    property alias echoMode : embTextInput.echoMode
    property alias validator : embTextInput.validator
    property alias horizontalAlignment : embTextInput.horizontalAlignment
    property alias verticalAlignment : embTextInput.verticalAlignment
    property alias radius : embRect.radius
    property alias border : embRect.border
    property alias imgSource : embImage.source
    property alias inputMethodHints : embTextInput.inputMethodHints
    //property alias activeFocus : embTextInput.activeFocus
    signal editingFinished
    signal accepted

    activeFocusOnTab: true

    Rectangle{
        id:embRect
        radius : 5
        opacity: 1
        height:parent.height
        width: parent.width
        color:"black"
        border.color: embTextInput.activeFocus ? "#428bca" : "#999999"
        border.width: embTextInput.activeFocus ? 2 : 1


        Image{
            id:embImage
            property real imgRatio : sourceSize ? sourceSize.height / sourceSize.width :1
            visible: source!=null
            height : parent.height * 0.3
            width : height * imgRatio
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 0.5*parent.radius
            anchors.left: parent.left

            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
        }

        TextInput{
            id:embTextInput
            text: ""
            maximumLength: 25

            anchors.leftMargin: 0.5*parent.radius
            anchors.left: embImage.right
            anchors.right: embRect.right
            anchors.top: embRect.top
            anchors.bottom: embRect.bottom

            //anchors.centerIn: embRect
            verticalAlignment: TextInput.AlignVCenter
            horizontalAlignment: TextInput.AlignHCenter
            //height:parent.height
            //width: parent.width
            focus: true
            font.pixelSize: 16 * root.scaleFactor

            color: "white"
            onEditingFinished: textField.editingFinished();
            onAccepted: textField.accepted()
            selectionColor : Qt.darker( embRect.color)
            Text {
                id: placeholder
                text:"enter your text here"
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: parent.horizontalAlignment//Text.AlignHCenter

                minimumPixelSize: 8
                fontSizeMode : Text.Fit
                visible: !(parent.text.length || embTextInput.inputMethodComposing)
                font: parent.font
                color: "#aaa"
            }

        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                embRect.forceActiveFocus()
                Qt.inputMethod.show()
            }
        }
        function forceActiveFocus()
        {
            embTextInput.forceActiveFocus()
            embTextInput.selectAll()
        }

    }



    onAccepted: {
        Qt.inputMethod.commit();
        Qt.inputMethod.hide();
    }
}

