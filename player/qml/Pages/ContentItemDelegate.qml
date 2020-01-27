import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0
import MyComponents 1.0

Item {
    id: delegate
    width: delegate.GridView.view.cellWidth
    height: delegate.GridView.view.cellHeight
    clip: true

    ColumnLayout {
        anchors { fill: parent; margins: 10 }
        spacing: 0

        Image {
            id: image
            Layout.alignment: Qt.AlignHCenter
            fillMode: Image.PreserveAspectFit
            height: (parent.height-text.height)*0.8
            sourceSize.height: height
            source: albumArtURI != "" ? albumArtURI : "image://upnpclass/" + upnpClass
            asynchronous: true
            clip: true

            onStatusChanged: {
                // albumArtURI cannot be loaded
                if (image.status == Image.Error && source == albumArtURI)
                    source = "image://upnpclass/" + upnpClass
            }
        }

        Label {
            id: text
            Layout.alignment: Qt.AlignHCenter
            horizontalAlignment: Text.AlignHCenter
            height: contentHeight
            text: title
            Layout.fillWidth: true
            elide: Text.ElideRight
            wrapMode: Text.WordWrap
            clip: true
        }
    }

    MouseArea {
        id: mousearea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: delegate.GridView.view.currentIndex = index
        onDoubleClicked: {
            if (upnpClass.startsWith("object.container"))
                delegate.GridView.view.setContentPath(objectId)
            else
                delegate.GridView.view.setContentItem(objectId, model)
        }
    }
}
