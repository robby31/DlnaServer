import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import MyComponents 1.0

Page {
    id: requests

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
            model: _app.requestsModel

            TableViewColumn {
                role: "date"
                title: "Date"
                width: 200
            }

            TableViewColumn {
                role: "status"
                title: "Status"
                width: 150
            }

            TableViewColumn {
                role: "network_status"
                title: "NetworkStatus"
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
                width: 150
            }

            TableViewColumn {
                role: "method"
                title: "Method"
                width: 120
            }

            TableViewColumn {
                role: "argument"
                title: "Argument"
                width: 200
            }

            onDoubleClicked: {
                request.requestIndex = currentRow
                tableView.visible = false
            }
        }

        ColumnLayout {
            id: requestColumn
            Layout.fillWidth: true
            height: 300
            visible: !tableView.visible

            Rectangle {
                id: cmd
                width: itemText.contentWidth*1.3
                height: itemText.contentHeight*1.3
                border.color: "black"
                border.width: 1
                radius: 10

                Text {
                    id: itemText
                    anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
                    text: "Back"
                    font.family: "Helvetica"
                    font.pointSize: 14
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: tableView.visible = true
                }
            }

            Request {
                id: request
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}
