import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import MyComponents 1.0

Page {
    id: shared

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
                    color: "black"
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

            Button {
                text: "Check"
                onClicked: _app.startCheckNetworkLink()
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

            Button {
                text: "Reload library"
                onClicked: _app.reloadLibrary()
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
