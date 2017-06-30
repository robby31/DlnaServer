import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import MyComponents 1.0

Page {
    id: renderers

    width: 600
    height: 300

    signal quit()

    actions: pageActions

    onActionClicked: {
        if (name == "Quit")
            quit()
    }

    ListModel {
        id: pageActions

        ListElement {
            name: "Quit"
            description: "exit application"
            icon: "qrc:///images/exit.png"
        }
    }

    ListView {
        id: listview
        anchors.fill: parent
        clip: true

        model: _app.renderersModel
        delegate: MediaRendererDelegate { }

        ScrollBar.vertical: ScrollBar { }
    }
}
