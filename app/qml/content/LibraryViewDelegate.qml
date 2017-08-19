import QtQuick 2.0

Item {
    id: delegate

    width: parent.width
    height: 20

    Text {
        anchors.fill: parent
        clip: true
        text: name
        font.pixelSize: 14
        font.bold: delegate.ListView.isCurrentItem
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        anchors.fill: parent
        onClicked: delegate.ListView.view.currentIndex = index
    }
}
