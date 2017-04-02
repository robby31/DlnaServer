import QtQuick 2.4
import QtQuick.Controls 1.3
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

        ScrollView {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListView {
                id: listview

                model: _app.requestsModel
                delegate: RequestDelegate { }

                function selectRequest(index) {
                    request.requestIndex = index
                    listview.visible = false
                }
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
                        font.pointSize: 12
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
