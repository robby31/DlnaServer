import QtQuick 2.0
import QtQuick.Controls 2.1
import MyComponents 1.0

ListViewDelegate {
    id: delegate
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
                text: model["id"] ? model["id"] : ""
                width: 100
                height: contentHeight
                anchors.verticalCenter: parent.verticalCenter
                clip: true
            }

            Text {
                id: textDate
                text: model["addedDate"] ? model["addedDate"] : ""
                width: 200
                height: contentHeight
                anchors.verticalCenter: parent.verticalCenter
                clip: true
            }

            Text {
                id: textFilename
                text: model["filename"] ? model["filename"] : ""
                width: (parent.width-textId.width)/2
                height: contentHeight
                anchors.verticalCenter: parent.verticalCenter
                clip: true
            }

            Text {
                id: textTitle
                text: model["title"] ? model["title"] : ""
                width: (parent.width-textId.width)/2
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideRight
                clip: true
            }
        }
    }
}

