import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import MyComponents 1.0
import Type 1.0

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

    StackView
    {
        id: view
        anchors.fill: parent
        initialItem: serverList
    }

    function goBack() {
        view.pop()
    }

    Component
    {
        id: serverList

        ListView {
            id: listview
            clip: true
            focus: true

            model: _app.serversModel
            delegate: ServerDelegate { }

            ScrollBar.vertical: ScrollBar { }

            function selectServer(index)
            {
                var server = model.getServer(index)
                view.push("ServerContent.qml", { server: server, contentModel: server.contentModel })
            }
        }
    }

}
