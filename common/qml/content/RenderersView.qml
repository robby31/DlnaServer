import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

Item {
    id: root
    width: 600
    height: 300
    anchors.fill: parent
    anchors.margins: Qt.platform.os === "osx" ? 12 : 6

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        spacing: 4

        TableView{
            id: tableView
            objectName: "RenderersTableView"
            Layout.fillHeight: true
            Layout.fillWidth: true
            anchors.margins: Qt.platform.os === "osx" ? 12 : 6
            model: renderersModel

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
