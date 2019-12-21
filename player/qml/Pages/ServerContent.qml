import QtQuick 2.0
import QtQuick.Controls 2.2
import MyComponents 1.0

Item {
    id: item

    property var server: null
    property alias contentModel: grid.model

    Column {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            width: parent.width
            height: 40

            gradient: Gradient {
                GradientStop { position: 0; color: theme.gradientStartColor }
                GradientStop { position: 1; color: theme.gradientEndColor }
            }

            Row {
                id: row
                anchors { left: parent.left; right: parent.right; margins: 10; verticalCenter: parent.verticalCenter }
                height: parent.height
                spacing: 10

                MyButton {
                    anchors { verticalCenter: parent.verticalCenter }
                    sourceComponent: Text { text: "< Back" }
                    onButtonClicked: goBack()
                }

                Image {
                    anchors { verticalCenter: parent.verticalCenter }
                    height: row.height*0.6
                    sourceSize.height: height
                    fillMode: Image.PreserveAspectFit
                    source: server.iconurl
                }


                Label {
                    anchors { verticalCenter: parent.verticalCenter }
                    text: server.name
                    color: "blue"
                }
            }
        }

        Component {
            id: highlight
            Rectangle {
                width: grid.cellWidth; height: grid.cellHeight
                color: "lightsteelblue"; radius: 5
                x: grid.currentItem.x
                y: grid.currentItem.y
                Behavior on x { SpringAnimation { spring: 3; damping: 0.2 } }
                Behavior on y { SpringAnimation { spring: 3; damping: 0.2 } }
            }
        }

        GridView {
            id: grid
            width: parent.width
            height: parent.height - row.height - parent.spacing
            cellWidth: 200
            cellHeight: 200

            clip: true
            focus: true
            antialiasing: true

            delegate: ContentItemDelegate { }

            highlight: highlight
            highlightFollowsCurrentItem: false

            ScrollBar.vertical: ScrollBar { }

            function setContentPath(objectId) {
                view.push("ServerContent.qml", { server: server, contentModel: server.contentModel.getChildrenModel(objectId) })
            }

            function setContentItem(objectId, data) {
                view.push("XmlItemContent.qml", { objectId: objectId, objectData: data })
            }
        }
    }
}
