import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtWebView 1.1
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

    StackView {
        id: stack
        initialItem: homeView
        anchors.fill: parent
    }

    Component {
        id: homeView

        GridView {
            id: grid

            cellWidth: 150
            cellHeight: 150

            focus: true
            clip: true

            ScrollBar.vertical: ScrollBar { }

            model: webModel
            delegate: delegateWebPage
            antialiasing: true

            highlight: highlight
            highlightFollowsCurrentItem: false

            Component {
                id: highlight
                Rectangle {
                    width: grid.cellWidth; height: grid.cellHeight
                    color: "lightsteelblue"; radius: 5
                    x: grid.currentItem.x
                    y: grid.currentItem.y
                    Behavior on x { SpringAnimation { spring: 3; damping: 0.2 } }
                    Behavior on y { SpringAnimation { spring: 3; damping: 0.2 } }
                }
            }
        }
    }

    ListModel {
        id: webModel
        ListElement { name: "Youtube"; url: "https://www.youtube.com"; icon: "qrc:///images/youtube.png" }
        ListElement { name: "Tf1"; url: "https://www.tf1.fr"; icon: "qrc:///images/mytf1.png" }
        ListElement { name: "FranceTv"; url: "https://www.france.tv"; icon: "qrc:///images/francetv.png" }
        ListElement { name: "ArteTv"; url: "https://www.arte.tv"; icon: "qrc:///images/arte.png" }
        ListElement { name: "M6"; url: "https://www.6play.fr"; icon: "qrc:///images/m6replay.png" }
        ListElement { name: "RmcDecouverte"; url: "https://rmcdecouverte.bfmtv.com"; icon: "qrc:///images/rmcdecouverte.png" }
        ListElement { name: "DailyMotion"; url: "https://www.dailymotion.com"; icon: "qrc:///images/dailymotion.png" }
    }

    Component {
        id: delegateWebPage

        Item {
            id: delegate
            width: delegate.GridView.view.cellWidth
            height: delegate.GridView.view.cellHeight

            ColumnLayout {
                id: column
                width: delegate.GridView.view.cellWidth - 20
                height: delegate.GridView.view.cellHeight - 20
                anchors { verticalCenter:  parent.verticalCenter; horizontalCenter: parent.horizontalCenter }
                spacing: 5

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Image {
                        anchors { verticalCenter:  parent.verticalCenter; horizontalCenter: parent.horizontalCenter }
                        fillMode: Image.PreserveAspectFit
                        width: parent.height - 20
                        source: icon ? icon : "qrc:///images/internet.png"
                        asynchronous: false
                        clip: true
                    }
                }

                Text {
                    id: textItem
                    Layout.alignment: Qt.AlignHCenter
                    horizontalAlignment: Text.AlignHCenter
                    width: parent.width
                    height: contentHeight
                    text: name
                    font.bold: true
                    elide: Text.ElideRight
                    clip: true
                }
            }

            MouseArea {
                id: mousearea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: delegate.GridView.view.currentIndex = index
                onDoubleClicked: stack.push(webNavigator, {url: url})
            }
        }
    }

    Component {
        id: webNavigator

        Item {
            property alias url: webview.url

            ColumnLayout {
                anchors.fill: parent
                spacing: 4

                RowLayout {
                    Layout.preferredHeight: 30
                    spacing: 4

                    Button {
                        Layout.preferredHeight: 20
                        text: "Home"
                        onClicked: stack.pop()
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
    }
}
