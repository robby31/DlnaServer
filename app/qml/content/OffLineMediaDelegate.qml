import QtQuick 2.0
import QtQuick.Controls 2.1
import MyComponents 1.0

ListViewDelegate {
    id: delegate
    width: parent ? parent.width : 0
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
            if (!delegate.ListView.view.model.remove(index))
                swipe.close()
            else
                delegate.ListView.view.model.select()

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
                text: model["id"] ? model["id"] : ""
                width: 100
                height: contentHeight
                anchors.verticalCenter: parent.verticalCenter
                clip: true
            }

            ModelEditableText {
                id: textFilename
                width: (parent.width-textId.width)/2
                anchors.verticalCenter: parent.verticalCenter

                textRole: "filename"
                placeholderText: "unknown filename"

                onEditingFinished: _app.updateFilenameMedia(id, text)

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
