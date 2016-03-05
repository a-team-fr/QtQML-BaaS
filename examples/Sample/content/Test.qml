import QtQuick 2.5
import BaaS 1.0
import "../controls"


Item {
    id: root
    width: parent.width
    height: parent.height
    property var lstColumnLabels: ["name", "cost","qty"]

    ListModel {
        id: fruitModel
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



    TestListView{
        model: fruitModel
        roles: lstColumnLabels
        width: parent.width
        height: parent.height
        useTestModel:true
    }

    SPSButtonText{
        text:"reload"
        width:100; height:30
        onClicked: fruitModel.resetModel()

    }
}
