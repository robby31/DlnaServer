import QtQuick 2.3
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1
import QtWebView 1.0
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
            Layout.preferredHeight: 30
            spacing: 4

            Button {
                Layout.preferredHeight: 20
                text: "Home"
                onClicked: webview.url = url
            }

            Button {
                Layout.preferredHeight: 20
                text: "Prev"
                enabled: webview.canGoBack
                onClicked: webview.goBack()
            }

            Button {
                Layout.preferredHeight: 20
                text: "Next"
                enabled: webview.canGoForward
                onClicked: webview.goForward()
            }

            Button {
                Layout.preferredHeight: 20
                text: "Stop"
                onClicked: webview.stop()
            }

            Button {
                Layout.preferredHeight: 20
                text: "Refresh"
                onClicked: webview.reload()
            }

            Button {
                Layout.preferredHeight: 20
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
