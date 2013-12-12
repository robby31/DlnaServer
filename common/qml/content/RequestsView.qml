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
            objectName: "RequestsTableView"
            Layout.fillHeight: true
            Layout.fillWidth: true
            model: requestsModel

            TableViewColumn {
                role: "date"
                title: "Date"
                width: 200
            }

            TableViewColumn {
                role: "status"
                title: "Status"
                width: 120
            }

            TableViewColumn {
                role: "duration"
                title: "ResponseTime"
                width: 120
            }

            TableViewColumn {
                role: "peerAddress"
                title: "Client"
                width: 120
            }

            TableViewColumn {
                role: "host"
                title: "Host"
                width: 120
            }

            TableViewColumn {
                role: "method"
                title: "Method"
                width: 120
            }

            TableViewColumn {
                role: "argument"
                title: "Argument"
                width: 120
            }

            onClicked: {
                header.text = requestsModel.get(currentRow, 5);
                content.text = requestsModel.get(currentRow, 6);
            }


        }

        RowLayout
        {
            TextArea {
                id: header
                Layout.fillWidth: true
            }

            TextArea {
                id: content
                Layout.fillWidth: true
            }
        }
    }
}
