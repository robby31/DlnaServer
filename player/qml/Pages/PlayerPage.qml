import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import QtMultimedia 5.9
import QtGraphicalEffects 1.0
import MyComponents 1.0

Page {
    id: page
    width: 600
    height: 300

    property var objectData
    property int duration

    states: [
        State {
            name: "LOADED w/o playlist"
            when: player.status === MediaPlayer.Loaded && player.playlist == null
            PropertyChanges { target: playButton; source: "qrc:/images/player/play.png"; enabled: true }
            PropertyChanges { target: stopButton; enabled: true }
            PropertyChanges { target: textStatus; text: "LOADED w/o playlist" }
        },

        State {
            name: "LOADED with playlist"
            when: player.status === MediaPlayer.Loaded && player.playlist != null
            PropertyChanges { target: backButton; enabled: true }
            PropertyChanges { target: playButton; source: "qrc:/images/player/play.png"; enabled: true }
            PropertyChanges { target: stopButton; enabled: true }
            PropertyChanges { target: nextButton; enabled: true }
            PropertyChanges { target: textStatus; text: "LOADED with playlist" }
        },

        State {
            name: "PLAY w/o playlist"
            when: player.playbackState == MediaPlayer.PlayingState && player.playlist == null
            PropertyChanges { target: playButton; source: "qrc:/images/player/pause.png"; enabled: true }
            PropertyChanges { target: stopButton; enabled: true }
            PropertyChanges { target: textStatus; text: "PLAY w/o playlist" }
        },

        State {
            name: "PLAY with playlist"
            when: player.playbackState == MediaPlayer.PlayingState && player.playlist != null
            PropertyChanges { target: backButton; enabled: true }
            PropertyChanges { target: playButton; source: "qrc:/images/player/pause.png"; enabled: true }
            PropertyChanges { target: stopButton; enabled: true }
            PropertyChanges { target: nextButton; enabled: true }
            PropertyChanges { target: textStatus; text: "PLAY with playlist" }
        },

        State {
            name: "PAUSE w/o playlist"
            when: player.playbackState == MediaPlayer.PausedState && player.playlist == null
            PropertyChanges { target: playButton; source: "qrc:/images/player/play.png"; enabled: true }
            PropertyChanges { target: stopButton; enabled: true }
            PropertyChanges { target: textStatus; text: "PAUSE w/o playlist" }
        },

        State {
            name: "PAUSE with playlist"
            when: player.playbackState == MediaPlayer.PausedState && player.playlist != null
            PropertyChanges { target: backButton; enabled: true }
            PropertyChanges { target: playButton; source: "qrc:/images/player/play.png"; enabled: true }
            PropertyChanges { target: stopButton; enabled: true }
            PropertyChanges { target: nextButton; enabled: true }
            PropertyChanges { target: textStatus; text: "PAUSE with playlist" }
        }
    ]

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

    ColumnLayout {
        anchors { fill: parent; margins: 10 }
        spacing: 0

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            VideoOutput {
                source: player
                visible: !albumArt.visible
            }

            Image {
                id: albumArt
                anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
                height: parent.height
                sourceSize.height: height
                fillMode: Image.PreserveAspectFit
                visible: albumArt.status === Image.Ready
            }
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text:  player.metaData.title ? player.metaData.title : (objectData && objectData["title"] ? objectData["title"] : "<no title>")
            width: contentWidth
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            font.bold: true
            clip: true
        }

        Row {
            Layout.alignment: Qt.AlignHCenter
            height: 40
            spacing: 10

            Text {
                anchors.verticalCenter: parent.verticalCenter
                text:  player.metaData.albumArtist ? player.metaData.albumArtist : (objectData && objectData["artist"] ? objectData["artist"] : "<no artist>")
                width: contentWidth
                height: parent.height
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                clip: true
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                text:  player.metaData.albumTitle ? player.metaData.albumTitle : (objectData && objectData["album"] ? objectData["album"] : "<no album>")
                width: contentWidth
                height: parent.height
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                clip: true
            }
        }

        Row {
            Layout.alignment: Qt.AlignHCenter
            height: 40
            spacing: 10

            Text {
                anchors.verticalCenter: parent.verticalCenter
                width: contentWidth
                text: formatTime(player.position) + " / " + (page.duration !== 0 ? formatTime(page.duration) : "-")
                height: parent.height
                verticalAlignment: Text.AlignVCenter
                visible: page.duration > 0
            }

            ProgressBar {
                anchors.verticalCenter: parent.verticalCenter
                width: 200
                to: page.duration
                value: player.position
                visible: to > 0
            }
        }

        Row {
            Layout.alignment: Qt.AlignHCenter
            height: 50
            spacing: 10

            ImageButton {
                id: backButton
                height: parent.height
                source: "qrc:/images/player/backward.png"
                enabled: false
            }

            ImageButton {
                id: playButton
                height: parent.height
                source: "qrc:/images/player/play.png"
                enabled: false

                onClicked: {
                    if (player.playbackState === MediaPlayer.PlayingState)
                        player.pause()
                    else
                        player.play()
                }
            }

            ImageButton {
                id: stopButton
                height: parent.height
                source: "qrc:/images/player/stop.png"
                enabled: false

                onClicked: player.stop()
            }

            ImageButton {
                id: nextButton
                height: parent.height
                source: "qrc:/images/player/forward.png"
                enabled: false
            }
        }

        Text {
            id: textStatus
            text: "unknown"
        }
    }

    onObjectDataChanged: {
        if (objectData["res"]) {
            player.source = objectData["res"]
            page.duration = objectData["duration"]
            if (objectData["albumArtURI"])
                albumArt.source = objectData["albumArtURI"]
            else
                albumArt.source = "image://upnpclass/" + objectData["upnpClass"]
        } else {
            console.log("invalid url", objectData["res"])
        }
    }

    MediaPlayer {
        id: player
        autoPlay: false
        onErrorStringChanged: console.log("ERROR", errorString)

        onDurationChanged: {
            if (duration != 0)
                page.duration = duration
        }
    }
}
