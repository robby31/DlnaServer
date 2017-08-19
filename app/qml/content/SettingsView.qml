import QtQuick 2.4
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import MyComponents 1.0
import QtQuick.Dialogs 1.2

Page {
    id: settingsPage

    width: 600
    height: 300

    Row {
        anchors { left: parent.left; leftMargin: 10; right: parent.right; rightMargin: 10; top: parent.top; topMargin: 10 }
        height: 40
        spacing: 10
        clip: true

        Label {
            anchors.verticalCenter: parent.verticalCenter
            text: "ffmpeg:"
        }

        Column {
            spacing: 5

            Label {
                text: _app.ffmpegFolder
            }

            Label {
                text: _app.ffmpegVersion
            }
        }

        Button {
            height: 20
            anchors.verticalCenter: parent.verticalCenter
            text: "select"
            onClicked: chooseFolderDialog.open()
        }
    }

    FileDialog {
        id: chooseFolderDialog
        selectExisting: true
        selectFolder: true
        onAccepted:  _app.ffmpegFolder = fileUrl
    }

}
