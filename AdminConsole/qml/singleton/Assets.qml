import QtQuick 2.0

pragma Singleton

Item {
    property var ui:ui
    property var fonts:fonts
    property var nav:nav

    Item{
        id:nav
        property string curPage: ""
    }

    Item{
        id:fonts
        property FontLoader awesome: FontLoader{ source: "qrc:/fonts/fontawesome-webfont.ttf" }
        //property FontLoader material: FontLoader{ source: "qrc:/fonts/MaterialIcons-Regular.ttf" }
    }

    Item{
        id:ui
        //Material config
        readonly property color accent: aTPink
        readonly property color primary: aTBlack
        readonly property color foreground: aTYellow
        readonly property color background: aTBlue

        readonly property color aTBlue: "#459ab3"
        readonly property color aTBlack:"#171B3E"
        readonly property color aTPink : "#bb2273"
        readonly property color aTYellow : "#e1ab0f"
        readonly property color aTLightBlue : "#93B7D8"
        readonly property color aTFlashyGreen : "#37FF9B"
        readonly property color aTGrey : "#595959"

        readonly property int maximumPixelSize : 72
        readonly property int minimumPixelSize : 8


    }
}




