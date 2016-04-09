import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    width: 600
    height: 400

    property int requestIndex: -1

    onRequestIndexChanged: setContent()

    function setContent() {
        if (requestIndex>=0 && listView.currentIndex>=0)
        {
            if (listView.currentIndex == 0)
                textArea.text = _app.requestsModel.get(requestIndex, "header")
            else if (listView.currentIndex == 1)
                textArea.text = _app.requestsModel.get(requestIndex, "content")
            else if (listView.currentIndex == 2)
                textArea.text = _app.requestsModel.get(requestIndex, "answer")
            else if (listView.currentIndex == 3)
                textArea.text = _app.requestsModel.get(requestIndex, "transcode_log")
            else
                textArea.text = ""
        }
    }

    Component {
        id: requestDelegate
        Item {
            id: itemDelegate
            width: 180
            height: textItem.height

            Text {
                id: textItem
                text: title
            }

            MouseArea {
                id: itemMouseArea
                anchors.fill: parent
                anchors.rightMargin: 0
                onClicked: {
                    listView.currentIndex = index
                }
            }
        }
    }

    ListModel {
        id: viewModel
        ListElement { title: "HEADER" }
        ListElement { title: "CONTENT" }
        ListElement { title: "ANSWER" }
        ListElement { title: "LOG" }
    }

    RowLayout {
        anchors.fill: parent

        ListView {
            id: listView
            width: 200
            Layout.fillHeight: true

            model: viewModel
            delegate: requestDelegate
            highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
            highlightFollowsCurrentItem: true
            focus: true

            onCurrentIndexChanged: setContent()
        }

        TextArea {
            id: textArea
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
