import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import MyComponents 1.0

Page {
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

        model: _app.serversModel
        delegate: ServerDelegate { }

        ScrollBar.vertical: ScrollBar { }

        function selectServer(index)
        {
            var server = model.get(index)
            console.log("select SERVER", server.name, server.networkAddress)
        }
    }
}
