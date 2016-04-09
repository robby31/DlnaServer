import QtQuick 2.3
import QtQuick.Controls 1.2
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

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        spacing: 4

        TableView{
            id: tableView
            Layout.fillHeight: true
            Layout.fillWidth: true
            anchors.margins: Qt.platform.os === "osx" ? 12 : 6
            model: _app.renderersModel

            TableViewColumn {
                role: "networkAddress"
                title: "IP"
                width: 200
            }

            TableViewColumn {
                role: "status"
                title: "Status"
                width: 200
            }

            TableViewColumn {
                role: "name"
                title: "Name"
                width: 300
            }

            TableViewColumn {
                role: "userAgent"
                title: "UserAgent"
                width: 300
            }
        }
    }
}
