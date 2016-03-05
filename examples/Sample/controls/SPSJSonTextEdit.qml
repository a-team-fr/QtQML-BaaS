import QtQuick 2.0

Item {
    property alias color : background.color
    property alias textColor : embText.color
    property alias text : embText.text
    property alias readOnly : embText.readOnly

    Flickable{
        id:flick
        clip: true
        anchors.fill: parent
        anchors.margins: 5
        contentWidth: embText.paintedWidth
        contentHeight: embText.paintedHeight
        TextEdit{
            id:embText
            text : ""
            color: "black"
            width: flick.width; height: flick.height
            wrapMode: TextEdit.Wrap
            selectByMouse:true
            textFormat: TextEdit.RichText
            onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
        }

        function ensureVisible(r)
        {
         if (contentX >= r.x)
             contentX = r.x;
         else if (contentX+width <= r.x+r.width)
             contentX = r.x+r.width-width;
         if (contentY >= r.y)
             contentY = r.y;
         else if (contentY+height <= r.y+r.height)
             contentY = r.y+r.height-height;
        }
    }
    Rectangle {
        id: scrollbar
        anchors.right: flick.right
        y: flick.visibleArea.yPosition * flick.height
        width: 10
        height: flick.visibleArea.heightRatio * flick.height
        color: "black"
    }

    Rectangle{
        id:background
        color:"white"
        radius:5
        z:-1
        width: parent.width; height: parent.height

    }




}
