import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3
import BaaS.Example.Assets 1.0

Label {
    property bool autoFit : false
    font.pointSize: Assets.ui.maximumPointSize
    minimumPointSize: Assets.ui.minimumPointSize
    fontSizeMode : Text.Fit
    horizontalAlignment: Qt.AlignHCenter
    verticalAlignment : Qt.AlignVCenter

}
