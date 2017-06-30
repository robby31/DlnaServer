import QtQuick 2.5
import QtQuick.Controls 2.1
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
                color: view.currentIndex == index ? theme.highlightSelectColor : "#00000000"
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

        visible: !checklinkview.visible

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
                text: "Refresh"
                onClicked: _app.refreshFolder(view.currentIndex)
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

            ScrollBar.vertical: ScrollBar { }

            model: _app.sharedFolderModel
            delegate: folderDelegate
            preferredHighlightEnd: 150
            preferredHighlightBegin: 50
        }
    }

    Component {
        id: checklinkDelegate

        Item {
            id: delegate
            width: parent.width
            height: 40

            Rectangle {
                id: hover
                anchors.fill: parent
                color: theme.hoverColor
                visible: mouseArea.containsMouse
            }

            Rectangle {
                id: highlight
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop { position: 0.0; color: theme.highlightGradientStart }
                    GradientStop { position: 1.0; color: theme.highlightGradientEnd }
                }
                visible: mouseArea.pressed
            }

            Rectangle {
                id: selected
                anchors.fill: parent
                color: theme.highlightSelectColor
                visible: delegate.ListView.isCurrentItem
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: checklinklistview.currentIndex = index
            }

            Row {
                width: parent.width-10
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                spacing: 5

                Text {
                    id: textMessage
                    text: model["message"]
                    width: contentWidth
                    anchors.verticalCenter: parent.verticalCenter
                    clip: true

                    color: model["message"] === "put ONLINE" ? "green" : (model["message"] === "put OFFLINE" ? "red" : "blue")
                }

                Text {
                    id: textName
                    text: model["name"]
                    width: parent.width-textMessage.width
                    height: contentHeight
                    anchors.verticalCenter: parent.verticalCenter
                    clip: true
                }
            }

            Rectangle {
                id: separatorBottom
                width: parent.width
                height: 1
                anchors { left: parent.left; bottom: parent.bottom }
                color: theme.separatorColor
            }
        }
    }

    ColumnLayout {
        id: checklinkview
        anchors.fill: parent
        spacing: 4
        visible: checklinklistview.model !== null

        Rectangle {
            id: header
            Layout.fillWidth: true
            height: 40

            gradient: Gradient {
                GradientStop { position: 0.0; color: theme.gradientStartColor }
                GradientStop { position: 1.0; color: theme.gradientEndColor }
            }

            MyButton {
                id: closeButton
                anchors { right: header.right; rightMargin: 10; verticalCenter: parent.verticalCenter }
                hovered: true
                sourceComponent: Text { width: contentWidth; height: contentHeight; text: "Close" }
                onButtonClicked: _app.closeCheckLink()
            }
        }

        ListView {
            id: checklinklistview
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true

            ScrollBar.vertical: ScrollBar { }

            model: _app.checkNetworkLinkModel
            delegate: checklinkDelegate
            preferredHighlightEnd: 150
            preferredHighlightBegin: 50

        }
    }

    BusyIndicator {
        id: busyindicator
        anchors.fill: parent
        activity: "Check Network links"
        activityProgress: _app.checkInProgress
        visible: _app.checkInProgress >= 0
        onCancel: _app.abortCheckLink()
    }
}
