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
                delegate.ListView.view.model.remove(index)
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
                text: model["id"]
                Layout.preferredWidth: 50
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideRight
                clip: true
            }

            Text {
                text: model["name"]
                Layout.preferredWidth: 200
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideRight
                clip: true
            }

            Text {
                text: model["sortname"]
                Layout.preferredWidth: 200
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideRight
                clip: true
            }

            Text {
                text: model["mediaCount"] + " medias"
                color: "blue"
                Layout.preferredWidth: 200
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideRight
                clip: true
            }

            Text {
                text: model["albumCount"] + " albums"
                color: "blue"
                Layout.preferredWidth: 200
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideRight
                clip: true
            }
        }
    }
}

