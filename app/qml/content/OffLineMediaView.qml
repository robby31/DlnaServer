import QtQuick 2.0
import QtQuick.Controls 2.1
import MyComponents 1.0

Rectangle {

    SqlListModel {
        id: mediaModel
        connectionName: "MEDIA_DATABASE"
        tablename: "media"
        query: "SELECT id, filename, title from media where is_reachable=0"
    }

    ListView {
        id: listView
        anchors.fill: parent
        clip: true
        focus: true

        ScrollBar.vertical: ScrollBar { }

        model: mediaModel
        delegate: OffLineMediaDelegate { }
        antialiasing: true
    }
}
