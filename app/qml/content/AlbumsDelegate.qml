import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import MyComponents 1.0

Item {
    id: delegate

    width: delegate.GridView.view.cellWidth
    height: delegate.GridView.view.cellHeight

    clip:true

    Column {
        id: column

        width: delegate.GridView.view.cellWidth - 20
        height: delegate.GridView.view.cellHeight - 20

        anchors { verticalCenter:  parent.verticalCenter; horizontalCenter: parent.horizontalCenter }

        spacing: 5

        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            fillMode: Image.PreserveAspectFit
            sourceSize.height: parent.height-5*textItem.height-5*column.spacing
            source: "image://album/" + albumId
            asynchronous: false
            clip: true
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            width: parent.width
            text: albumId
            font.bold: true
            elide: Text.ElideRight
            clip: true
        }

        Text {
            id: textItem
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            width: parent.width
            text: albumName ? albumName : "<No Name>"
            font.bold: true
            elide: Text.ElideRight
            clip: true
        }

        Text {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            text: artistName ? artistName : "<No Artist>"
            anchors.horizontalCenter: parent.horizontalCenter
            elide: Text.ElideRight
            clip: true
        }

        Text {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            text: year ? year : "<No Year>"
            anchors.horizontalCenter: parent.horizontalCenter
            elide: Text.ElideRight
            clip: true
        }

        Text {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            text: mediaCount
            anchors.horizontalCenter: parent.horizontalCenter
            elide: Text.ElideRight
            clip: true
        }
    }

    MouseArea {
        id: mousearea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: delegate.GridView.view.currentIndex = index
        onDoubleClicked: openAlbum(id)
    }

    Image {
        anchors { top: parent.top; topMargin: 10; right: parent.right; rightMargin: 10 }
        width: 20
        fillMode: Image.PreserveAspectFit
        source: "qrc:///images/clear.png"
        visible: mediaCount==0 && delegate.GridView.isCurrentItem && mousearea.containsMouse

        MouseArea {
            anchors.fill: parent
            onClicked: delegate.GridView.view.removeAlbum(index)
        }
    }
}

