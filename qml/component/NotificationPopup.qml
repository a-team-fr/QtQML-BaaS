import QtQuick 2.11
import QtQuick.Controls 2.4

ToolTip{
    id:root
    timeout: 4000
    property bool showOnTextChange : true
    onTextChanged: showOnTextChange && (text !="") ? open() : close();
    implicitWidth: 200
    implicitHeight: 30
    margins:0
    padding : 0
    property color backgroundColor : "green"
    background: Rectangle {

          border.color: "white"
          color : root.backgroundColor
          opacity:0.7
      }
    contentItem:ATText{
        horizontalAlignment: Qt.AlignLeft
        text:root.text
    }
}
