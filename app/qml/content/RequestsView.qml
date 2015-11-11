import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

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

            onCurrentRowChanged: tabviewAnswer.updateContent(currentRow)
        }

        TabView {
            id: tabviewAnswer
            Layout.fillHeight: true
            Layout.fillWidth: true
            anchors.margins: Qt.platform.os === "osx" ? 12 : 6
            property int currentModelRow: -1;

            function updateContent(currentRow) {
                currentModelRow = currentRow
                if (currentModelRow != -1) {
                    if (currentIndex == 0)
                    {
                        tabHeader.item.text = _app.requestsModel.get(currentRow, "header")
                    } else if (currentIndex == 1) {
                        tabContent.item.text = _app.requestsModel.get(currentRow, "content")
                    } else if (currentIndex == 2) {
                        tabAnswer.item.text = _app.requestsModel.get(currentRow, "answer")
                    } else if (currentIndex == 3) {
                        tabLog.item.text = _app.requestsModel.get(currentRow, "transcode_log")
                    }
                }
            }

            onCurrentIndexChanged: updateContent(currentModelRow)

            Tab {
                id: tabHeader
                title: "HEADER"
                TextArea {
                    Layout.fillWidth: true
                }
            }

            Tab {
                id: tabContent
                title: "CONTENT"
                TextArea {
                    Layout.fillWidth: true
                }
            }

            Tab {
                id: tabAnswer
                title: "ANSWER"
                TextArea {
                    Layout.fillWidth: true
                }
            }

            Tab {
                id: tabLog
                title: "LOG"
                TextArea {
                    Layout.fillWidth: true
                }
            }
        }
    }
}
