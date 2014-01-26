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

    TabView {
        id:frame
        anchors.fill: parent
        anchors.margins: Qt.platform.os === "osx" ? 12 : 6

        Tab {
            title: "Request"
            RequestsView { }
        }

        Tab {
            title: "Renderers"
            RenderersView { }
        }

    }

}
