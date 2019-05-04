import QtQuick 2.5
import QtQuick.Controls 2.1
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
            width: parent.width
            height: textItem.height

            Text {
                id: textItem
                text: title
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
            }

            MouseArea {
                id: itemMouseArea
                anchors.fill: parent
                onClicked: listView.currentIndex = index
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
        spacing: 5

        ListView {
            id: listView
            Layout.preferredWidth: 200
            Layout.fillHeight: true
            clip: true

            ScrollBar.vertical: ScrollBar { }

            model: viewModel
            delegate: requestDelegate
            highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
            highlightFollowsCurrentItem: true
            focus: true

            onCurrentIndexChanged: setContent()
        }

        Rectangle {
            Layout.preferredWidth: 1
            Layout.fillHeight: true
            color: theme.separatorColor
        }

        Flickable {
            Layout.fillWidth: true
            Layout.fillHeight: true

            TextArea.flickable: TextArea { id: textArea; readOnly: true; selectByMouse: true }

            ScrollBar.vertical: ScrollBar { }
        }
    }
}
