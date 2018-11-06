import QtQuick 2.6
import QtQuick.Controls 2.1
import ATeam.AbriService.AssetsSingleton 1.0
import QtQuick.Controls.Material 2.1

Text {
    //width: Assets.ui.defaultWidth
    //height:Assets.ui.defaultHeight
    color: Material.foreground
    //color: Assets.ui.textcolor
    font.pixelSize: Assets.ui.maximumPixelSize
    minimumPixelSize: Assets.ui.minimumPixelSize
    fontSizeMode : Text.Fit
    horizontalAlignment: Qt.AlignHCenter
    verticalAlignment : Qt.AlignVCenter

}
