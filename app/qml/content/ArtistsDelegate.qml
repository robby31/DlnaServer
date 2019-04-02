import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import MyComponents 1.0

ListViewDelegate {
    id: delegate
    width: parent ? parent.width : 0
    height: 40

    swipe.right: Row {
        height: parent.height
        anchors.right: parent.right
        spacing: 0

        Label {
            id: deleteLabel
            text: qsTr("Delete")
            color: "white"
            verticalAlignment: Label.AlignVCenter
            padding: 12
            height: parent.height

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
    }

    contentItem: Item {
        width: parent.width
        height: 40

        RowLayout {
            width: parent.width-10
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            spacing: 5

            Text {
                text: artistId != null ? artistId : "<invalid>"
                Layout.preferredWidth: 50
                Layout.alignment: Qt.AlignVCenter
                elide: Text.ElideRight
                clip: true
            }

            ModelEditableText {
                text: name != null ? name : "<invalid>"
                placeholderText: "unknown name"
                Layout.preferredWidth: 200
                Layout.alignment: Qt.AlignVCenter
                onEditingFinished: {
                    name = text
                    focus = false
                }
                clip: true
            }

            ModelEditableText {
                text: sortname != null ? sortname : "<invalid>"
                placeholderText: "unknown sortname"
                Layout.preferredWidth: 200
                Layout.alignment: Qt.AlignVCenter
                onEditingFinished: {
                    sortname = text
                    focus = false
                }
                clip: true
            }

            Text {
                text: mediaCount != null ? mediaCount + " medias" : "<invalid>"
                color: "blue"
                Layout.preferredWidth: 200
                Layout.alignment: Qt.AlignVCenter
                elide: Text.ElideRight
                clip: true
            }

            Text {
                text: albumCount != null ? albumCount + " albums" : "<invalid>"
                color: "blue"
                Layout.preferredWidth: 200
                Layout.alignment: Qt.AlignVCenter
                elide: Text.ElideRight
                clip: true
            }
        }
    }
}

