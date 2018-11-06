import QtQuick 2.6
import QtQuick.Controls 2.1
import BaaS.AdminConsole.Assets 1.0
import QtQuick.Controls.Material 2.1

Text {
    color: Material.foreground
    font.pixelSize: Assets.ui.maximumPixelSize
    minimumPixelSize: Assets.ui.minimumPixelSize
    fontSizeMode : Text.Fit
    horizontalAlignment: Qt.AlignHCenter
    verticalAlignment : Qt.AlignVCenter

}
