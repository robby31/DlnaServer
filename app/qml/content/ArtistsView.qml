import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import MyComponents 1.0

Rectangle {

    SqlListModel {
        id: mediaModel
        connectionName: "MEDIA_DATABASE"
        tablename: "artist"

        function filter(cmd) {
            var strQuery
            strQuery = "SELECT id, id AS artistId, name, sortname, (SELECT count(media.id) from media WHERE media.artist=artist.id) AS mediaCount, (SELECT count(album.id) from album WHERE album.artist=artist.id) AS albumCount from artist "
            if (cmd)
                strQuery += "WHERE %1 ".arg(cmd)
            strQuery += "ORDER BY name"
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
                        text: mediaModel.rowCount + " artists"
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

        ListView {
            id: listView

            width: parent.width
            height: parent.height - rowLayout.height

            focus: true
            clip: true

            ScrollBar.vertical: ScrollBar { }

            model: mediaModel
            delegate: ArtistsDelegate { }
            antialiasing: true

            Component.onCompleted: {
                if (model)
                {
                    model.filter("")
                    if (model.rowCount > 0)
                        currentIndex = 0
                }
            }
        }
    }
}
