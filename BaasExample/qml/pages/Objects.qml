import QtQuick 2.11
import QtQuick.Controls 2.4
import BaaS.Example.Assets 1.0
import QtQuick.Layouts 1.3
import BaaS 1.0

import "../component"

Flickable {
    anchors.fill:parent
    contentHeight: content.contentItem.height
    contentWidth : width

    BaaSModel{
        id: baasmodel
        source:Assets.backend
        endPoint:""
    }
    ATTableView{
        id:content
        model: baasmodel
        roles: baasmodel.rolesList
        testMode: false
        width:parent.width
        height : parent.height
        showRemove : true

        onRemove: model.remove(row)//Assets.backend.deleteUser(baasmodel.get(row,"objectId"))
    }


}
