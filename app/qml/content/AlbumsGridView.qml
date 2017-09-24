import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import MyComponents 1.0

Rectangle {

    SqlListModel {
        id: mediaModel
        connectionName: "MEDIA_DATABASE"
        tablename: "album"

        function filter(cmd) {
            var strQuery
            strQuery = "SELECT album.id, album.year, album.name AS albumName, artist.name AS artistName, (SELECT count(media.id) from media WHERE media.album=album.id) AS mediaCount from album LEFT OUTER JOIN artist ON album.artist=artist.id "
            if (cmd)
                strQuery += "WHERE %1 ".arg(cmd)
            strQuery += "ORDER BY artist.name, album.year"
            query = strQuery
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
                    anchors.verticalCenter: parent.verticalCenter
                    clip: true
                    placeholderText: "Filtering"
                    selectByMouse: true
                    onAccepted: mediaModel.filter(text)

                    background: Rectangle {
                        color: "white"
                        border.color: parent.focus ? "#21be2b" : "grey"
                    }
                }

                Row {
                    anchors.verticalCenter: parent.verticalCenter
                    Layout.fillWidth: true
                    layoutDirection: Qt.RightToLeft
                    spacing: 5
                    clip: true

                    Text {
                        width: contentWidth
                        text: "albums"
                        color: "blue"
                        clip: true
                    }

                    Text {
                        width: contentWidth
                        text: mediaModel.rowCount
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
                    model.filter("")
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
