import QtQuick 2.0
import QtQuick.Controls 2.2
import MyComponents 1.0

Item {
    id: item

    property string objectId: ""
    property var objectData: null

    Column {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            width: parent.width
            height: 40

            gradient: Gradient {
                GradientStop { position: 0; color: theme.gradientStartColor }
                GradientStop { position: 1; color: theme.gradientEndColor }
            }

            Row {
                id: row
                anchors { left: parent.left; right: parent.right; margins: 10; verticalCenter: parent.verticalCenter }
                height: parent.height
                spacing: 10

                MyButton {
                    anchors { verticalCenter: parent.verticalCenter }
                    sourceComponent: Text { text: "< Back" }
                    onButtonClicked: goBack()
                }

                Image {
                    anchors { verticalCenter: parent.verticalCenter }
                    height: row.height*0.6
                    sourceSize.height: height
                    fillMode: Image.PreserveAspectFit
                    source: objectData["albumArtURI"] ? objectData["albumArtURI"] : "image://upnpclass/" + objectData["upnpClass"]
                }


                Label {
                    anchors { verticalCenter: parent.verticalCenter }
                    text: objectData["title"]
                    color: "blue"
                }
            }
        }

        Component {
            id: highlight
            Rectangle {
                width: grid.cellWidth; height: grid.cellHeight
                color: "lightsteelblue"; radius: 5
                x: grid.currentItem.x
                y: grid.currentItem.y
                Behavior on x { SpringAnimation { spring: 3; damping: 0.2 } }
                Behavior on y { SpringAnimation { spring: 3; damping: 0.2 } }
            }
        }

        Label {
            text: "id: " + objectData["objectId"] + ", parentID: " + objectData["parentID"]
        }

        Label {
            text: "class: " + objectData["upnpClass"]
        }

        Label {
            text: "title: " + objectData["title"]
        }

        Label {
            text: "artist: " + objectData["artist"]
        }

        Label {
            text: "album: " + objectData["album"]
        }

        Label {
            text: "genre: " + objectData["genre"]
        }

        Label {
            text: "date: " + objectData["date"]
        }

        Label {
            text: "url :" + objectData["res"]
        }

        Label {
            text: "protocolInfo :" + objectData["res@protocolInfo"]
        }

        Label {
            text: "duration :" + objectData["res@duration"]
        }

        Label {
            text: "size :" + objectData["res@size"]
        }
    }
}
