import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import MyComponents 1.0
import Model 1.0

Rectangle {

    MediaModel {
        id: mediaModel
        table: "playlists"
        query: "SELECT id, name, url, (SELECT count(media_in_playlists.id) from media_in_playlists WHERE media_in_playlists.playlist=playlists.id) AS mediaCount from playlists"
        orderClause: "ORDER BY name"

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
                        text: mediaModel.rowCount + " playlists"
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
            delegate: PlaylistDelegate { }
            antialiasing: true

            Component.onCompleted: {
                if (model)
                {
                    model.filterCmd("")
                    if (model.rowCount > 0)
                        currentIndex = 0
                }
            }
        }
    }
}
