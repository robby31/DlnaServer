import QtQuick 2.3
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import QtWebKit 3.0
import MyComponents 1.0

Page {
    id: navigator

    width: 100
    height: 62

    signal quit()

    actions: pageActions

    onActionClicked: {
        if (name == "Quit")
            quit()
    }

    ListModel {
        id: pageActions

        ListElement {
            name: "Quit"
            description: "exit application"
            icon: "qrc:///images/exit.png"
        }
    }

    property string url: "http://www.youtube.com"

    Component.onCompleted: webview.url = url

    ColumnLayout {
        anchors.fill: parent
        spacing: 4

        RowLayout {
            spacing: 4

            Button {
                text: "Home"
                onClicked: webview.url = url
            }

            Button {
                text: "Prev"
                enabled: webview.canGoBack
                onClicked: webview.goBack()
            }

            Button {
                text: "Next"
                enabled: webview.canGoForward
                onClicked: webview.goForward()
            }

            Button {
                text: "Stop"
                onClicked: webview.stop()
            }

            Button {
                text: "Refresh"
                onClicked: webview.reload()
            }

            Button {
                text: "Add to library"
                onClicked: _app.addNetworkLink(webview.url)
            }
        }

        WebView {
            id: webview
            Layout.fillHeight: true
            Layout.fillWidth: true

        }

    }
}
