import QtQuick 2.1
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1

Item {
    id: root
    width: 600
    height: 300
    anchors.fill: parent
    anchors.margins: Qt.platform.os === "osx" ? 12 : 6

    FileDialog {
        id: fileDialog
        title: "Please choose a folder"
        selectFolder: true
        selectExisting: true
        selectMultiple: false

        onAccepted: _app.addSharedFolder(fileDialog.fileUrl)
    }

    Component {
        id: folderDelegate

        Item {
            anchors.right: parent.right
            anchors.left: parent.left
            height: 50

            Rectangle {
                id: frame
                anchors.fill: parent
                anchors.rightMargin: 5
                anchors.leftMargin: 5
                anchors.topMargin: 5
                anchors.bottomMargin: 5
                color: view.currentIndex == index ? "lightsteelblue" : "#00000000"
                radius: parent.height/4
                border.width: 2

                MouseArea {
                    id: itemMouseArea
                    anchors.fill: parent
                    anchors.rightMargin: 0
                    onClicked: {
                        view.currentIndex = index
                    }
                }

                Text {
                    id: text
                    color: "#ffffff"
                    text: modelData
                    anchors.centerIn: parent
                    font.pointSize: 18
                }
            }
        }
    }

    ColumnLayout {
        id: mainLayout
        width: 600
        height: 300
        anchors.fill: parent
        spacing: 4

        RowLayout {
            spacing: 4

            TextField {
                id: link
                Layout.fillWidth: true
                placeholderText: "Network Link, example: https://www.youtube.com/watch?v=rWy30tJdyAM"
            }

            Button {
                text: "Add link"
                onClicked: _app.addNetworkLink(link.text)
            }
        }

        RowLayout {
            spacing: 4

            Button {
                text: "Add"
                onClicked: fileDialog.open()
            }

            Button {
                text: "Remove"
                onClicked: _app.removeFolder(view.currentIndex)
            }
        }

        ListView {
            id: view
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            model: _app.sharedFolderModel
            delegate: folderDelegate
            preferredHighlightEnd: 150
            preferredHighlightBegin: 50
        }

    }
}
