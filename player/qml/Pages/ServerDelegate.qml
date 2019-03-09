import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0
import MyComponents 1.0

ListViewDelegate {
    id: delegate
    height: 50

    property color color: available ? "black" : "red"

    contentItem: Item {
        RowLayout {
            anchors { left: parent.left; right: arrow.left; margins: 10; verticalCenter: parent.verticalCenter }
            spacing: 10

            Image {
                Layout.alignment: Qt.AlignVCenter
                height: delegate.height*0.6
                sourceSize.height: height
                fillMode: Image.PreserveAspectFit
                source: iconurl
            }

            Label {
                Layout.alignment: Qt.AlignVCenter
                text: name
                Layout.fillWidth: true
                elide: Text.ElideRight
                color: delegate.color
            }

            Label {
                Layout.alignment: Qt.AlignVCenter
                text: networkAddress
                width: 100
                Layout.preferredWidth: width
                elide: Text.ElideRight
                color: delegate.color
            }
        }

        Image {
            id: arrow
            anchors { right: parent.right; verticalCenter: parent.verticalCenter }
            height: parent.height
            fillMode: Image.PreserveAspectFit
            source: "qrc:/images/arrow.png"
        }
    }

    onClicked: delegate.ListView.view.selectServer(index)
}
