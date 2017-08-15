import QtQuick 2.0
import QtQuick.Controls 2.1
import MyComponents 1.0

ListViewDelegate {
    id: delegate
    width: parent.width
    height: 40

    swipe.left: Label {
        id: deleteLabel
        text: qsTr("Delete")
        color: "white"
        verticalAlignment: Label.AlignVCenter
        padding: 12
        height: parent.height
        anchors.left: parent.left

        SwipeDelegate.onClicked: {
            _app.removeMedia(model["id"])
            mediaModel.reload()
            swipe.close()
        }

        background: Rectangle {
            color: deleteLabel.SwipeDelegate.pressed ? Qt.darker("tomato", 1.1) : "tomato"
        }
    }

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
                width: 100
                height: contentHeight
                anchors.verticalCenter: parent.verticalCenter
                clip: true
            }

            TextField {
                id: textFilename
                text: model["filename"]
                width: (parent.width-textId.width)/2
                anchors.verticalCenter: parent.verticalCenter
                selectByMouse: true

                background: Rectangle {
                    color: textFilename.focus ? "white" : "transparent"
                    border.color: textFilename.focus ? "#21be2b" : "transparent"
                }

                onAccepted: _app.updateFilenameMedia(id, text)
            }

            Text {
                id: textTitle
                text: model["title"]
                width: (parent.width-textId.width)/2
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideRight
                clip: true
            }
        }
    }
}
