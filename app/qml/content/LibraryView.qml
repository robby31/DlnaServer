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
        ListElement { name: "Artists"; qml: "ArtistsView.qml" }
        ListElement { name: "Albums"; qml: "AlbumsView.qml" }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        ListView {
            id: menuView
            Layout.preferredWidth: 100
            Layout.fillHeight: true
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

        Rectangle {
            Layout.preferredWidth: 1
            Layout.fillHeight: true
            color: theme.separatorColor
        }

        Loader {
            id: viewLoader
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height
            focus: true
        }
    }
}
