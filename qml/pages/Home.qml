import QtQuick 2.11
import QtQuick.Controls 2.4
import FontAwesome 1.0
import "../component"


Pane{
    id: root
    anchors.fill:parent
    SwipeView {
        id: view
        clip:true
        currentIndex: 0
        anchors.fill:parent
        anchors.bottomMargin: indicator.height
        Flickable
        {
            contentWidth: width
            contentHeight:contentItem.childrenRect.height
            Label{

                width:parent.width
                wrapMode: Text.WordWrap
                text:qsTr("<h1>BaasSample %1</h1> <p>This is a sample application to showcase the BaaS (Backend As A Service) library which is attempt to provide a lightweight REST API to use a mobile backend (such as Firebase, Parse Server, AWS...) without being locked in one solution.</p> <h2>Current status</h2> <p>Work in progress ! It is currently only and partially supporting Parse Server..:</p><ul> <li>Athentification (without social login)</li> <li>datastore (without offline mode)</li> </ul>").arg(appVersion)
            }
        }


        Label{
            wrapMode: Text.WordWrap
            text:qsTr("<h1>Quick start</h1>")
        }

    }
    PageIndicator {
        id: indicator
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        height: 20

        count: view.count
        currentIndex: view.currentIndex
    }
}



