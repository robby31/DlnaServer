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
            anchors.margins: Qt.platform.os === "osx" ? 12 : 6
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
                        tabHeader.item.text = requestsModel.get(currentRow, 5)
                    } else if (currentIndex == 1) {
                        tabAnswer.item.text = requestsModel.get(currentRow, 6)
                    } else if (currentIndex == 2) {
                        tabContent.item.text = requestsModel.get(currentRow, 9)
                    } else if (currentIndex == 3) {
                        tabTranscodeLog.item.text = requestsModel.get(currentRow, 11)
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
                id: tabAnswer
                title: "ANSWER"
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
                id: tabTranscodeLog
                title: "TRANSCODING"
                TextArea {
                    Layout.fillWidth: true
                }
            }
        }
    }
}
