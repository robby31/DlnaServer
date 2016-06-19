import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import MyComponents 1.0

Page {
    id: debugPage

    width: 600
    height: 300

    signal quit()

    actions: pageActions

    onActionClicked: {
        if (name == "Reload")
            reloadPage()
        else if (name == "Quit")
            quit()
    }

    function reloadPage() {
        listView.model = null
        listView.model = _app.debugModel
    }

    ListModel {
        id: pageActions

        ListElement {
            name: "Reload"
            description: "Reload screen"
            icon: "qrc:///images/exit.png"
        }

        ListElement {
            name: "Quit"
            description: "exit application"
            icon: "qrc:///images/exit.png"
        }
    }

    Component {
        id: debugDelegate

        Row {
            Text {
                id: textName
                text: model["name"]
                width: 400
                height: contentHeight
            }

            Text {
                id: textCounter
                text: model["counter"]
                width: 100
                height: contentHeight
            }
        }
    }

    ListView {
        id: listView
        anchors.fill: parent
        model: _app.debugModel
        delegate: debugDelegate
    }

    Timer {
        interval: 1000
        running: listView.visible
        repeat: true
        onTriggered: reloadPage()
    }
}
