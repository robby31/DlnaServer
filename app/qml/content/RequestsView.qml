import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import MyComponents 1.0

Page {
    id: requests

    width: 600
    height: 300

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
        anchors { fill: parent; margins: 10 }
        initialItem: listView
    }

    Component {
        id: listView

        ListView {
            clip: true

            ScrollBar.vertical: ScrollBar { }

            model: _app.requestsModel
            delegate: RequestDelegate { }

            function selectRequest(index) {
                stack.push(requestView, {index: index})
            }
        }
    }

    Component {
        id: requestView

        ColumnLayout {
            property alias index: request.requestIndex

            MyButton {
                id: cmd

                sourceComponent: Text {
                    text: "Back"
                    font.pixelSize: 12
                    clip: true
                }

                onButtonClicked: stack.pop()
            }

            Request {
                id: request
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}
