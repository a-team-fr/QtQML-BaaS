import QtQuick 2.11
import QtQuick.Controls 2.4
import BaaS.Example.Assets 1.0
import QtQuick.Layouts 1.11
import FontAwesome 1.0
import "../component"

Pane {
    anchors.fill:parent
    anchors.margins : 10
    contentHeight: view.childrenRect.height
    contentWidth : Math.max(width, view.childrenRect.width)


    SwipeView {
         id: view
         clip:true
         currentIndex: 0
         anchors.fill: parent

         Page {
            Label{
                 anchors.fill: parent
                 wrapMode: Text.WordWrap
                 text:"<h1>BaasSample</h1> <p>This is a sample application to showcase the BaaS (Backend As A Service) library which is attempt to provide a lightweight REST API to use a mobile backend (such as Firebase, Parse Server, AWS...) without being locked in one solution.</p> <h2>Current status</h2> <p>Work in progress ! It is currently only and partially supporting Parse Server..:</p><ul> <li>Athentification (without social login)</li> <li>datastore (without offline mode)</li> </ul>"
             }
         }
         Page {
             Label{
                 anchors.fill: parent
                 wrapMode: Text.WordWrap
                 text:"<h1>Quick start</h1>"
             }
         }

     }
    PageIndicator {
          id: indicator

          count: view.count
          currentIndex: view.currentIndex

          anchors.bottom: view.bottom
          anchors.horizontalCenter: parent.horizontalCenter
      }


}
