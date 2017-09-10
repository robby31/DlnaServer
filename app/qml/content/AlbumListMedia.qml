import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import MyComponents 1.0

Rectangle {

    property int idAlbum: -1

    SqlListModel {
        id: mediaModel
        connectionName: "MEDIA_DATABASE"
        tablename: "media"

        function filter(cmd) {
            var strQuery
            strQuery = "SELECT media.id, media.picture, filename, format, type.name AS mediaType, title, media.artist, artist.name AS artistName, album.name AS albumName from media "
            strQuery += "LEFT OUTER JOIN artist ON media.artist=artist.id "
            strQuery += "LEFT OUTER JOIN album ON media.album=album.id "
            strQuery += "LEFT OUTER JOIN type ON media.type=type.id "
            if (cmd)
                strQuery += "WHERE media.album=%1 and %2".arg(idAlbum).arg(cmd)
            else
                strQuery += "WHERE media.album=%1".arg(idAlbum)
            query = strQuery
        }

        Component.onCompleted: filter("")
    }

    SqlListModel {
        id: artistModel
        connectionName: "MEDIA_DATABASE"
        tablename: "artist"
        query: "SELECT * from artist"

        function getArtistId(name) {
            query = "SELECT id from artist WHERE name='%1'".arg(name)
            if (rowCount > 0)
                return get(0, "name")
            else
                return -1
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

                MyButton {
                    anchors.verticalCenter: parent.verticalCenter
                    sourceComponent: Text { text: "< Albums" }
                    onButtonClicked: goBack()
                }

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
                    spacing: 10
                    clip: true

                    Text {
                        width: contentWidth
                        text: mediaModel.rowCount + " media"
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
            delegate: AllMediaDelegate { }
            antialiasing: true
        }
    }
}
