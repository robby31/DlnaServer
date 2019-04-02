import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import MyComponents 1.0
import Model 1.0

Rectangle {

    MediaModel {
        id: mediaModel
        table: "album"
        query: "SELECT album.id, album.id AS albumId, album.year, album.name AS albumName, artist.name AS artistName, (SELECT count(media.id) from media WHERE media.album=album.id) AS mediaCount from album"
        join: "LEFT OUTER JOIN artist ON album.artist=artist.id"
        orderClause: "ORDER BY album.name, artist.name, album.year"

        function filterCmd(cmd) {
            filter = cmd
            select()
        }
    }

    Column {
        id: mainLayout
        anchors.fill: parent
        spacing: 0

        Rectangle {
            width: parent.width
            height: 40

            gradient: Gradient {
                GradientStop { position: 0.0; color: theme.gradientStartColor }
                GradientStop { position: 1.0; color: theme.gradientEndColor }
            }

            RowLayout {
                id: rowLayout
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                height: parent.height
                spacing: 10

                TextField {
                    id: textFilter
                    Layout.preferredWidth: 400
                    Layout.preferredHeight: 30
                    Layout.alignment: Qt.AlignVCenter
                    clip: true
                    placeholderText: "Filtering"
                    selectByMouse: true
                    onAccepted: mediaModel.filterCmd(text)

                    background: Rectangle {
                        color: "white"
                        border.color: parent.focus ? "#21be2b" : "grey"
                    }
                }

                Row {
                    Layout.alignment: Qt.AlignVCenter
                    Layout.fillWidth: true
                    layoutDirection: Qt.RightToLeft
                    spacing: 5
                    clip: true

                    Text {
                        width: contentWidth
                        text: mediaModel.rowCount + " albums"
                        color: "blue"
                        clip: true
                    }
                }

            }
        }

        Rectangle {
            id: separatorBottom
            width: parent.width
            height: 1
            color: theme.separatorColor
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

        GridView {
            id: grid

            width: parent.width
            height: parent.height - rowLayout.height

            cellWidth: 250
            cellHeight: 200

            focus: true
            clip: true

            ScrollBar.vertical: ScrollBar { }

            model: mediaModel
            delegate: AlbumsDelegate { }
            antialiasing: true

            highlight: highlight
            highlightFollowsCurrentItem: false

            Component.onCompleted: {
                if (model)
                {
                    model.filterCmd("")
                    if (model.rowCount > 0)
                        currentIndex = 0
                }
            }

            function removeAlbum(index) {
                model.remove(index)
            }
        }
    }
}

