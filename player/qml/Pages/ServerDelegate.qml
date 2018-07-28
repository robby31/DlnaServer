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
            anchors { left: parent.left; right: parent.right; margins: 10; verticalCenter: parent.verticalCenter }
            spacing: 10

            Image {
                anchors { verticalCenter: parent.verticalCenter }
                height: delegate.height*0.6
                sourceSize.height: height
                fillMode: Image.PreserveAspectFit
                source: iconurl
            }

            Label {
                anchors { verticalCenter: parent.verticalCenter }
                text: networkAddress
                width: 150
                Layout.preferredWidth: width
                elide: Text.ElideRight
                color: delegate.color
            }

            Label {
                anchors { verticalCenter: parent.verticalCenter }
                text: name
                Layout.fillWidth: true
                elide: Text.ElideRight
                color: delegate.color
            }

            Label {
                anchors { verticalCenter: parent.verticalCenter }
                text: status
                width: (parent.width-320)/2
                Layout.preferredWidth: width
                elide: Text.ElideRight
                color: delegate.color
            }
        }
    }

    onClicked: delegate.ListView.view.selectServer(index)
}
