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
            id: updateLabel
            text: qsTr("Update")
            color: "white"
            verticalAlignment: Label.AlignVCenter
            padding: 12
            height: parent.height

            SwipeDelegate.onClicked: {
                swipe.close()
                _app.update_playlist(url)
            }

            background: Rectangle {
                color: updateLabel.SwipeDelegate.pressed ? Qt.darker("tomato", 1.1) : "tomato"
            }
        }

        Label {
            id: exportLabel
            text: qsTr("Export")
            color: "white"
            verticalAlignment: Label.AlignVCenter
            padding: 12
            height: parent.height

            SwipeDelegate.onClicked: {
                swipe.close()
                _app.export_playlist(url)
            }

            background: Rectangle {
                color: exportLabel.SwipeDelegate.pressed ? Qt.darker("tomato", 1.1) : "tomato"
            }
        }

//        Label {
//            id: deleteLabel
//            text: qsTr("Delete")
//            color: "white"
//            verticalAlignment: Label.AlignVCenter
//            padding: 12
//            height: parent.height

//            SwipeDelegate.onClicked: {
//                if (!delegate.ListView.view.model.remove(index))
//                    swipe.close()
//                else
//                    delegate.ListView.view.model.select()
//            }

//            background: Rectangle {
//                color: deleteLabel.SwipeDelegate.pressed ? Qt.darker("tomato", 1.1) : "tomato"
//            }
//        }
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
                text: model["id"] !== null ? model["id"] : "<invalid>"
                Layout.preferredWidth: 50
                Layout.alignment: Qt.AlignVCenter
                elide: Text.ElideRight
                clip: true
            }

            ModelEditableText {
                textRole: "name"
                placeholderText: "unknown name"
                Layout.preferredWidth: 200
                Layout.alignment: Qt.AlignVCenter
                clip: true
            }

            ModelEditableText {
                textRole: "url"
                placeholderText: "unknown url"
                Layout.preferredWidth: 500
                Layout.alignment: Qt.AlignVCenter
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
        }
    }
}


