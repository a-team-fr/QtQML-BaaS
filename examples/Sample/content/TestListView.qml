import QtQuick 2.5


Item {
    id: root

    property var model: null
    property var roles: null
    property int rowMinHeight : 20
    property int visibleRows : 4
    property color textColor : "white"
    property color bgColor : "darkgrey"
    property bool showHeader : true
    property bool showRemove : true

    signal remove(int row);

    Component{
        id:defaultDelegate
        Row{
            id:row
            spacing: 1
            property int indexSaved : index
            Repeater {
                model: root.roles
                delegate: Rectangle {
                        radius: 3
                        width: listView.ceilWidth
                        height: listView.ceilHeight
                        color: root.bgColor
                        border.color: Qt.darker(root.bgColor)
                        border.width: 1
                        Text {
                            text: root.model.get(row.indexSaved ,modelData)
                            anchors.fill: parent
                            color: root.textColor
                            font.pixelSize: 14
                            minimumPixelSize: 8
                            fontSizeMode : Text.Fit
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment : Text.AlignVCenter
                        }
                }
            }
            Text {
                visible: root.showRemove
                text: "X"
                width:50; height:parent.height;
                color: root.textColor
                fontSizeMode : Text.Fit
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment : Text.AlignVCenter
                MouseArea{
                    anchors.fill: parent
                    onClicked:remove(row.indexSaved)
                }
            }
            //TODO Add handler modify/delete
        }
    }
    Component{
        id:defaultHeader
        Row{
            spacing: 1
            z:2
            Repeater {
                model: root.roles
                delegate: Rectangle {
                        radius: 3
                        width: listView.ceilWidth
                        height: listView.ceilHeight
                        color: Qt.darker( root.bgColor)
                        border.color: Qt.lighter(root.bgColor)
                        border.width: 1
                        Text {
                            text: modelData
                            anchors.fill: parent
                            color: Qt.lighter( root.textColor)
                            font.pixelSize: 20
                            minimumPixelSize: 10
                            fontSizeMode : Text.Fit
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment : Text.AlignVCenter
                        }
                }
            }
        }
    }

    ListView {
        id:listView
        model: root.model
        spacing: 0
        width: parent.width
        height: parent.height
        contentWidth: width
        contentHeight: contentItem.childrenRect.height
        headerPositioning: ListView.OverlayHeader

        clip:true
        property int visibleRows : root.showHeader ? root.visibleRows + 1 : root.visibleRows
        property int ceilWidth : root.showRemove ? (width - 50) / root.roles.length : width / root.roles.length
        property int ceilHeight : root.visibleRows > 0 ? Math.max(height / visibleRows, root.rowMinHeight) : root.rowMinHeight

        header: root.showHeader ? defaultHeader : null
        delegate : defaultDelegate
        //TODO add "create new"

    }

}

