import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import MyComponents 1.0

StackView {
    id: stack

    initialItem: Qt.resolvedUrl("AlbumsGridView.qml")

    function goBack() {
        stack.pop()
    }

    function openAlbum(id) {
        stack.push("AlbumListMedia.qml", {idAlbum: id})
    }
}

