import QtQuick 2.5

//pragma Singleton
Item {
    property var ui:ui

    Item{
        id:ui
        property color green:"#A2C64C"
        property color darkBlue:"#171B3E"
        property color background:"lightgrey"
        property color menuBackground:"darkblue"
        property color textcolor:"white"
        property color textEditColor:"white"
        property color buttonMenuBkColor:"lightgrey"
        property color buttonMenuBkColorDisabled:"grey"
        property color buttonBkColor:"#428bca"
        property color buttonBkColorDisabled:"grey"
        property color buttonTextcolor:"white"
        property real buttonBkOpacity:1
        property color borderColor:"grey"
        property color textInputBackground:"#171B3E"
        property real buttonRadiusPercHeight: 0.2 //Defines the perc of height to compute the corner radius
        property real backgroundRadiusPercHeight: 0.1
        property int buttonMargin: 10
        property int minimumPixelSize:8
        property int textInputDynSize: textM
        property real scaleFactor: 1
        property int textS:8*scaleFactor
        property int textM:16*scaleFactor
        property int textXL:32*scaleFactor
        property int textXXL:54*scaleFactor
        property int textXXXL:72*scaleFactor
        property int textGodzilla:100*scaleFactor
        property Gradient backgradient: Gradient{
            GradientStop{ position: 0; color:"darkblue";}
            GradientStop{ position: 0.6; color:"black";}
        }


    }

}
