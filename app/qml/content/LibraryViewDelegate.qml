import QtQuick 2.0

Item {
    id: delegate

    width: parent.width
    height: 20

    Text {
        anchors.fill: parent
        clip: true
        text: name
        font.pointSize: 9
        font.bold: delegate.ListView.isCurrentItem
        horizontalAlignment: Text.AlignHCenter
    }

    MouseArea {
        anchors.fill: parent
        onClicked: delegate.ListView.view.currentIndex = index
    }
}
