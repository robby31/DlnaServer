import QtQuick 2.4
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import MyComponents 1.0

Item {
    id: item

    property  alias model: listview.model

    Component {
        id: delegateItem

        ListViewDelegate {
            clip: true

            contentItem: Row {
                Text {
                    id: textName
                    anchors.verticalCenter: parent.verticalCenter
                    text: model.display
                    width: 400
                    height: contentHeight
                }
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent

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

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: item.model.rowCount() + " items"
                    width: 400
                    height: contentHeight
                    color: "blue"
                }
            }
        }

        ListView {
            id: listview
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            delegate: delegateItem
        }
    }
}
