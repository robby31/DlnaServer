import QtQuick 2.0
import QtQuick.Controls 2.1
import MyComponents 1.0

ListViewDelegate {
    id: delegate
    width: parent ? parent.width : 0
    height: 40

    contentItem: Item {
        id: item
        width: parent.width
        height: 40

        Row {
            width: parent.width-10
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            spacing: 5

            Text {
                id: textId
                text: model["id"]
                width: 50
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideRight
                clip: true
            }

            Text {
                id: textFilename
                text: model["filename"]
                width: (parent.width-textId.width)/3
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideRight
                clip: true
            }

            Text {
                id: textFormat
                text: model["format"]
                width: (parent.width-textId.width)/3
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideRight
                clip: true
            }

            Text {
                id: textTitle
                text: model["title"]
                width: (parent.width-textId.width)/3
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideRight
                clip: true
            }
        }
    }
}
