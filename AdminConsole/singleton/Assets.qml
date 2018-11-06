import QtQuick 2.0

pragma Singleton

Item {
    property var ui:ui
//    property var fonts:fonts


//    Item{
//        id:fonts
//        property FontLoader awesome: FontLoader{ source: "qrc:/fonts/fontawesome-webfont.ttf" }
//        //property FontLoader material: FontLoader{ source: "qrc:/fonts/MaterialIcons-Regular.ttf" }
//    }

    Item{
        id:ui
        //Material config
        property color accent: ATPink
        property color primary: ATBlack
        property color foreground: ATYellow
        property color background: ATBlue

        readonly property color ATBlue: "#459ab3"
        readonly property color ATBlack:"#171B3E"
        readonly property color ATPink : "#bb2273"
        readonly property color ATYellow : "#e1ab0f"
        readonly property color ATLightBlue : "#93B7D8"
        readonly property color ATFlashyGreen : "#37FF9B"
        readonly property color ATGrey : "#595959"

        readonly property int maximumPixelSize : 72
        readonly property int minimumPixelSize : 8


    }
}




