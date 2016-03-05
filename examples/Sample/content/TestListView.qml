import QtQuick 2.5


Item {
    id: root

    //TODO add a way to select columns to display, ordering
    property var model: null
    property var roles: null
    property int rowMinHeight : 20
    property int visibleRows : 4
    property color textColor : "white"
    property color bgColor : "darkgrey"
    property bool showHeader : true
    property bool showRemove : true

    property bool useTestModel : false  //This is only for testing purpose to use with a ListModel i/o BaasModel

    property alias currentIndex : listView.currentIndex
    signal remove(int row);
    signal rowClicked(int row);

    Component {
        id: highlight
        Rectangle {
            //z:3
            width: listView.width
            height: listView.ceilHeight
            //opacity:0.8
            color: "lightsteelblue"; radius: 5
            //y: listView.currentItem.y
            Behavior on y {
                SpringAnimation {
                    spring: 3
                    damping: 0.2
                }
            }
        }
    }

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
                        color: listView.currentIndex == row.indexSaved ? "transparent" : root.bgColor
                        border.color: Qt.darker(root.bgColor)
                        border.width: 1
                        Text {
                            text: root.useTestModel ? root.model.get(row.indexSaved)[modelData] : root.model.get(row.indexSaved ,modelData)
                            anchors.fill: parent
                            color: root.textColor
                            font.pixelSize: 14
                            minimumPixelSize: 8
                            fontSizeMode : Text.Fit
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment : Text.AlignVCenter
                            elide : Text.ElideRight
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                rowClicked( row.indexSaved )
                                listView.currentIndex = row.indexSaved
                            }
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
        //headerPositioning: ListView.OverlayHeader
        highlightFollowsCurrentItem: true
        //highlightRangeMode: ListView.StrictlyEnforceRange
        //snapMode: ListView.SnapToItem
        focus:true
        clip:true
        enabled: true
        property int visibleRows : root.showHeader ? root.visibleRows + 1 : root.visibleRows
        property int ceilWidth : root.roles.length > 0 ? (root.showRemove ? (width - 50) / root.roles.length : width / root.roles.length) : 1
        property int ceilHeight : root.visibleRows > 0 ? Math.max(height / visibleRows, root.rowMinHeight) : root.rowMinHeight

        header: root.showHeader ? defaultHeader : null
        delegate : defaultDelegate
        highlight: highlight



        //TODO add "create new"

    }

}

