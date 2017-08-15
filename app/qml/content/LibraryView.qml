import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.1
import MyComponents 1.0
import myTypes 1.0

Page {
    id: libraryView

    width: 600
    height: 300

    signal quit()

    actions: pageActions

    onActionClicked: {
        if (name == "Quit")
            quit()
        else if (name == "Reload")
        {
            mediaModel.reload()
        }
    }

    ListModel {
        id: pageActions

        ListElement {
            name: "Reload"
            description: "reload library"
            icon: "qrc:///images/exit.png"
        }

        ListElement {
            name: "Quit"
            description: "exit application"
            icon: "qrc:///images/exit.png"
        }

    }

    ListModel {
        id: viewModel
        ListElement { name: "All media"; qml: "AllMediaView.qml" }
        ListElement { name: "Off-Line"; qml: "OffLineMediaView.qml" }
    }

    Row {
        anchors.fill: parent

        Rectangle {
            width: 100
            height: parent.height
            border.color: "black"

            ListView {
                id: menuView
                anchors.fill: parent
                clip: true
                model: viewModel
                delegate: LibraryViewDelegate { }

                ScrollBar.vertical: ScrollBar { }

                highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
                highlightFollowsCurrentItem: true
                focus: true

                onCurrentIndexChanged: {
                    viewLoader.source = model.get(currentIndex).qml
                }
            }
        }

        Loader {
            id: viewLoader
            width: parent.width-menuView.width
            height: parent.height
        }
    }
}
