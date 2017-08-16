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

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        spacing: 4

        ListView {
            id: listview

            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true

            ScrollBar.vertical: ScrollBar { }

            model: _app.requestsModel
            delegate: RequestDelegate { }

            function selectRequest(index) {
                request.requestIndex = index
                listview.visible = false
            }
        }

        ColumnLayout {
            id: requestColumn
            anchors.fill: parent
            visible: !listview.visible

            RowLayout {
                height: cmd.height
                MyButton {
                    id: cmd

                    sourceComponent: Text {
                        text: "Back"
                        font.pixelSize: 12
                        clip: true
                    }

                    onButtonClicked: listview.visible = true
                }
            }

            Request {
                id: request
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}
