import QtQuick 2.11
import QtQuick.Controls 2.4
import BaaS.Example.Assets 1.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.1
import FontAwesome 1.0
import "./component"

ToolBar{
    id:root
    signal toggleDrawer();
    signal toggleMenu();
    //Material.foreground: "white"

    RowLayout{
        spacing: 10
        anchors.fill: parent

        ToolButton{
            font.family: FontAwesome.fontFamily
            text:FontAwesome.alignJustify
            onClicked: root.toggleDrawer()
            Layout.preferredWidth: 30

        }
        Label{
            //visible:Assets.backend.userName !==""
            Layout.fillWidth: true
            text:Assets.nav.curPage.label
            horizontalAlignment: Text.AlignHCenter
            elide : Text.ElideRight
        }


        ToolButton{
            font.family: FontAwesome.fontFamily
            text:FontAwesome.ellipsisV
            onClicked: root.toggleMenu()
            Layout.preferredWidth: 30

        }

        /*ToolButton{
            font.family: FontAwesome.fontFamily
            text:Assets.backend.useMaster ? FontAwesome.key : FontAwesome.userSecret
            visible: true//Assets.backend.masterKey !== ""
            onClicked: Assets.backend.useMaster = !Assets.backend.useMaster
            Layout.preferredWidth: 30
        }*/

    }
}
