import QtQuick 2.4
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import MyComponents 1.0

Page {
    id: debugPage

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

    Component {
        id: debugDelegate

        ListViewDelegate {
            clip: true

            contentItem: Row {
                Text {
                    id: textName
                    anchors.verticalCenter: parent.verticalCenter
                    text: model["name"]
                    width: 400
                    height: contentHeight
                }

                Text {
                    id: textCounter
                    anchors.verticalCenter: parent.verticalCenter
                    text: model["counter"]
                    width: 100
                    height: contentHeight
                }
            }

            onDoubleClicked: {
                var detailsModel = ListView.view.model.detailsModel(model["name"])
                stackView.push("DebugDetails.qml", {model: detailsModel})
            }
        }
    }

    function goBack() {
        stackView.pop()
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: homeView
    }

    Component {
        id: homeView

        ListView {
            clip: true
            model: _app.debugModel
            delegate: debugDelegate

            ScrollBar.vertical: ScrollBar { }
        }
    }
}
