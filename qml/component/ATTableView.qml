import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Window 2.11

ListView {
    id:listView


    //highlightRangeMode: ListView.ApplyRange
    //snapMode : ListView.SnapOneItem
    focus:true
    clip:true


    implicitWidth : 20 + ceilWidth * roles.length
    implicitHeight: ceilHeight * (count +1)

    property int ceilWidth : 150
    property int ceilHeight : 20

    headerPositioning : ListView.OverlayHeader
    header: Row{
        Repeater {
            model: listView.roles
            delegate: Rectangle {
                color : Qt.darker( listView.bgColor)
                border.color: "white"
                border.width: 1
                width : listView.ceilWidth
                height : listView.ceilHeight
                Label {
                    text: modelData
                    anchors.fill: parent; anchors.margins: 2
                    color: listView.textColor
                    elide: Text.ElideLeft

                }


            }
        }
    }
    delegate : defaultDelegate



    property bool testMode : false
    property var dummyRoles: ["name", "cost","qty"]
    ListModel {
        id: dummyModel
        ListElement {
            name: "Apple"
            cost: 2.45
            qty: "2"
        }
        ListElement {
            name: "Orange"
            cost: 3.25
            qty: "5"
        }
        ListElement {
            name: "Banana"
            cost: 1.95
            qty: "235"
        }
    }
    function getData(rowIdx, role)
    {
        if (testMode) return model.get(rowIdx) ? model.get(rowIdx)[role] : "";
        else return model.get(rowIdx, role);
    }

    Component.onCompleted: {
        if (testMode)
        {
            model = dummyModel;
            roles = dummyRoles;
        }
    }


    property var roles: null
    property color textColor : "white"
    property color bgColor : "darkgrey"
    property bool showRemove : true

    signal remove(int row);
    signal rowClicked(int row);


    Component{
        id:defaultDelegate
        Row{
            id : row
            property int rowIndex : index
            property bool isCurrent : ListView.isCurrentItem
            property var modelData : model
            Repeater {
                model: listView.roles
                delegate: Rectangle {
                    color : row.isCurrent ? "yellow" : listView.bgColor
                    border.color: row.isCurrent ? "yellow" : "black"
                    border.width: 1
                    width : listView.ceilWidth
                    height : listView.ceilHeight
                    Label {
                        id:ceilText
                        //text: listView.getData(row.rowIndex, modelData).toString()
                        text: listView.getData(row.rowIndex, modelData) ? listView.getData(row.rowIndex, modelData).toString() : ""
                        anchors.fill: parent; anchors.margins: 2
                        color: row.isCurrent ? "black" : listView.textColor
                        elide: Text.ElideLeft
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            listView.rowClicked( row.rowIndex )
                            listView.currentIndex = row.rowIndex
                        }
                        onPressAndHold: {
                            showText.text = ceilText.text
                            showText.open()
                        }
                    }
                }
            }
            Rectangle {
                color : Qt.darker( listView.bgColor)
                border.color: "white"
                border.width: 1
                visible : listView.showRemove
                width:20; height:listView.ceilHeight;
                ATText {
                    visible: listView.showRemove
                    text: "X"
                    color : listView.textColor
                    anchors.fill: parent; anchors.margins: 2
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked:remove(row.rowIndex)
                }
            }
        }
    }

    Dialog{
        id:showText
        modal : true
        //standardButtons: Dialog.Ok
        width : parent.width * 0.8
        height: parent.height * 0.8
        x : parent.width * 0.1
        y : parent.height * 0.1
        property string text : ""
        onAccepted : close()
        ATText{
            anchors.fill: parent
            text: showText.text


        }
    }

}

