import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.3
import BaaS.Example.Assets 1.0
import FontAwesome 1.0

import "./qml"
import "./qml/component"


ApplicationWindow {
    id:mainWnd
    visible: true
    width: Assets.ui.width
    height: Assets.ui.height
    title: qsTr("BaaS Showcase")

    header:Header{
        onToggleDrawer: drawer.open()
        //background:Rectangle{color:"darkgrey"}
        onToggleMenu:optionMenu.open()
    }


    Menu{
        x : parent.width - width
        id:optionMenu
        Action { text: footer.visible ? qsTr("hide footer") : qsTr("show footer"); onTriggered:footer.visible = !footer.visible}
        Action { text: qsTr("Privacy policy"); onTriggered:Qt.openUrlExternally("https://a-team.fr/acado/privacy_policy.html") }
        Action { text: qsTr("Terms and conditions"); onTriggered:Qt.openUrlExternally("https://a-team.fr/acado/terms_and_conditions.html") }

    }

    footer:Footer{
    }

    LeftDrawer{
        id:drawer
        height:mainWnd.height
        width:Math.min(mainWnd.height, parent.width) * 2 / 3
    }

    NotificationPopup{
        id:serverWait
        parent : ApplicationWindow.window.contentItem
        backgroundColor:"red"
        modal:true
        visible : Assets.backend.percComplete < 100
        showOnTextChange : false
        width:ApplicationWindow.window.width
        height:25
        timeout : 10000
        text:qsTr("please wait for server response...")

    }

    Loader{
        id:loader
        anchors.top : serverWait.bottom
        anchors.fill:parent
        anchors.bottomMargin: backendSuccessMessage.height
        source:Assets.nav.curPage ? Assets.nav.curPage.url : null

    }
    NotificationPopup
    {
        id:backendSuccessMessage
        parent : ApplicationWindow.window.contentItem
        y:loader.height
        width:ApplicationWindow.window.width
        height:20
        text:Assets.backend.successMessage
    }
    NotificationPopup
    {
        parent : ApplicationWindow.window.contentItem
        y:loader.height
        backgroundColor:"red"
        width:ApplicationWindow.window.width
        height:20
        text:Assets.backend.errorMessage
    }



    Toast{
        parent : ApplicationWindow.window.overlay
        iconTxt:FontAwesome.arrowLeft
        visible: Assets.nav.curPageIndex !== 0
        onClicked: Assets.nav.selectPage(0)
        draggable : true
        //default positioning, as item is draggable we should not use anchors
        x : parent.width - width - 5
        y : parent.height - ApplicationWindow.footer.height - height * 2 / 3
        z:1
        color : Material.primary
    }

}
