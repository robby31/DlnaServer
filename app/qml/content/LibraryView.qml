import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.1
import QtMultimedia 5.9
import MyComponents 1.0
import Model 1.0

Page {
    id: libraryView

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

    MediaModel {
        id: artistModel
        table: "artist"
        query: "SELECT id, name FROM artist"
        orderClause: "ORDER BY name"
        Component.onCompleted: select()
    }

    MediaModel {
        id: genreModel
        table: "genre"
        query: "SELECT id, name from genre"
        orderClause: "ORDER BY name"
        Component.onCompleted: select()
    }

    MediaModel {
        id: albumModel
        table: "album"
        query: "SELECT id, name from album"
        orderClause: "ORDER BY name"
        Component.onCompleted: select()
    }

    ListModel {
        id: viewModel
        ListElement { name: "All media"; qml: "AllMediaView.qml" }
        ListElement { name: "Off-Line"; qml: "OffLineMediaView.qml" }
        ListElement { name: "Recently played"; qml: "RecentlyPlayedMediaView.qml" }

        ListElement { name: "Artists"; qml: "ArtistsView.qml" }
        ListElement { name: "Albums"; qml: "AlbumsView.qml" }
    }

    function formatTime(time) {
        var minutes = Math.floor(time/60000)
        var seconds = Number(time/1000 - minutes*60).toFixed()

        var res = ""
        if (minutes < 10)
            res += "0%1".arg(minutes)
        else
            res += "%1".arg(minutes)

        res += ":"

        if (seconds < 10)
            res += "0%1".arg(seconds)
        else
            res += "%1".arg(seconds)

        return res
    }

    Rectangle {
        id: playerInfo
        anchors { top: parent.top; left: parent.left; right: parent.right }
        height: 40

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#4edab3" }
            GradientStop { position: 1.0; color: "#184337" }

        }

        Row {
            id: row
            anchors { horizontalCenter: parent.horizontalCenter; top: parent.top; bottom: parent.bottom }
            spacing: 10

            Text {
                id: title
                text: player.metaData.title ? player.metaData.title : ""
                width: contentWidth
                height: parent.height
                verticalAlignment: Text.AlignVCenter
                font.bold: true
            }

            Text {
                id: artist
                text: player.metaData.albumArtist ? player.metaData.albumArtist : ""
                width: contentWidth
                height: parent.height
                verticalAlignment: Text.AlignVCenter
                color: "blue"
            }

            Text {
                id: album
                text: player.metaData.albumTitle ? player.metaData.albumTitle : ""
                width: contentWidth
                height: parent.height
                verticalAlignment: Text.AlignVCenter
                color: "blue"
            }

            Text {
                id: status
                text: "status: " + player.status
                width: contentWidth
                height: parent.height
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                width: contentWidth
                text: formatTime(player.position) + " / " + formatTime(player.duration)
                height: parent.height
                verticalAlignment: Text.AlignVCenter
            }

            ProgressBar {
                anchors.verticalCenter: parent.verticalCenter
                width: 200
                to: player.duration
                value: player.position
            }
        }
    }

    RowLayout {
        anchors { top: playerInfo.bottom; left: parent.left; right: parent.right; bottom: parent.bottom }
        spacing: 0

        ListView {
            id: menuView
            Layout.preferredWidth: 200
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

    function playMedia(url) {
        player.source = url
    }

    MediaPlayer {
        id: player
        autoPlay: true
        onErrorStringChanged: console.log("ERROR", errorString)
    }
}
