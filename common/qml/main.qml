import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import "content"

ApplicationWindow {
    title: "QT Media Server"

    width: 600
    height: 400
    minimumHeight: 400
    minimumWidth: 570

    signal progressUpdate(int value)
    onProgressUpdate: {
        pb1.value = value
        pb1.visible = value < 100
    }

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        spacing: 4

        TabView {
            id:frame
            Layout.fillHeight: true
            Layout.fillWidth: true
            anchors.margins: Qt.platform.os === "osx" ? 12 : 6

            Tab {
                title: "Request"
                RequestsView { }
            }

            Tab {
                title: "Renderers"
                RenderersView { }
            }

            Tab {
                title: "Shared"
                SharedView { }
            }

        }

        // Progress bar defined
        ProgressBar {
            id: pb1
            Layout.fillWidth: true
            anchors.margins: Qt.platform.os === "osx" ? 12 : 6
            minimumValue: 0
            maximumValue: 100
            value: 0
        }
    }

}
