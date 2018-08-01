import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import QtMultimedia 5.9
import MyComponents 1.0

Page {
    id: page
    width: 600
    height: 300

    property var objectData
    property int duration

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
        anchors.fill: parent

        Rectangle {
            id: playerInfo
            Layout.fillWidth: true
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
                    text: objectData && objectData["title"] ? (player.metaData.title ? player.metaData.title : objectData["title"]) : "<no title>"
                    width: contentWidth
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    font.bold: true
                }

                Text {
                    id: artist
                    text: objectData &&  objectData["artist"] ? (player.metaData.albumArtist ? player.metaData.albumArtist : objectData["artist"]) : "<no artist>"
                    width: contentWidth
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    color: "blue"
                }

                Text {
                    id: album
                    text: objectData &&  objectData["album"] ? (player.metaData.albumTitle ? player.metaData.albumTitle : objectData["album"]) : "<no album>"
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
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            VideoOutput {
                source: player
                visible: !albumArt.visible
            }

            Image {
                id: albumArt
                Layout.fillWidth: true
                anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
                height: parent.height*0.8
                sourceSize.height: height
                fillMode: Image.PreserveAspectFit
                visible: albumArt.status === Image.Ready
            }
        }
    }

    onObjectDataChanged: {
        if (objectData["res"]) {
            player.source = objectData["res"]
            page.duration = objectData["duration"]
            albumArt.source = objectData["albumArtURI"]
        } else {
            console.log("invalid url", objectData["res"])
        }
    }

    MediaPlayer {
        id: player
        autoPlay: true
        onErrorStringChanged: console.log("ERROR", errorString)
        onDurationChanged: {
            if (duration != 0)
                page.duration = duration
        }
    }
}
