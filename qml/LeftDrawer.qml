import QtQuick 2.11
import QtQuick.Controls 2.4
import BaaS.Example.Assets 1.0
import QtQuick.Layouts 1.3
import "./component"

Drawer{
    id:drawer
    edge:Qt.LeftEdge

    ListView{
        id:lstView
        anchors.fill:parent
        model:Assets.nav.lstPages
        delegate:AwesomeButton{
            text:label
            iconTxt : model.iconTxt
            highlighted: index ===  Assets.nav.curPageIndex
            width:drawer.width
            height : 50
            iconElem.font.pointSize: 20
            onClicked: {
                drawer.close()
                Assets.nav.selectPage(index)
            }

        }
        ScrollIndicator.vertical: ScrollIndicator { }
    }






}
