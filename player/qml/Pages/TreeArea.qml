import QtQuick 2.0
import QtQuick.Controls 1.4

Item {
    id: item
    width: 600
    height: 200

    property alias model: view.model

    TreeView {
        id: view

        anchors.fill: parent

        alternatingRowColors: false
        backgroundVisible: false

        TableViewColumn {
            title: "Tag"
            role: "display"
            width: 300
        }

        itemDelegate: Rectangle {
            color: "transparent"
            Text {
                anchors.verticalCenter: parent.verticalCenter
                color: "black"
                elide: styleData.elideMode
                text: styleData.value
                textFormat: Text.PlainText
            }
        }
    }

}
