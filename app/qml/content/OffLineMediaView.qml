import QtQuick 2.0
import QtQuick.Controls 2.1
import MyComponents 1.0
import Model 1.0

Rectangle {

    MediaModel {
        id: mediaModel
        filter: "is_reachable=0"
        Component.onCompleted: select()
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
