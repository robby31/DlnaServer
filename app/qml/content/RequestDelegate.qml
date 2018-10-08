import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0

Item {
    id: delegate
    width: parent.width
    height: row.height

    property color color: "black"

    Rectangle {
        id: hover
        anchors.fill: parent
        color: theme.hoverColor
        visible: mouseArea.containsMouse
    }

    Rectangle {
        id: highlight
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: theme.highlightGradientStart }
            GradientStop { position: 1.0; color: theme.highlightGradientEnd }
        }
        visible: mouseArea.pressed
    }

    RowLayout {
        id: row
        anchors { left: parent.left; right: parent.right; margins: 10 }
        height: dateLabel.height+10
        spacing: 10

        Label {
            id: dateLabel
            Layout.alignment: Qt.AlignVCenter
            text: date
            width: contentWidth
            Layout.minimumWidth: contentWidth
            Layout.preferredWidth: width
            //elide: Text.ElideRight
            color: delegate.color
        }

        Label {
            Layout.alignment: Qt.AlignVCenter
            text: streaming_status ? streaming_status : status
            width: 150
            Layout.preferredWidth: width
            elide: Text.ElideRight
            color: delegate.color
        }

        Label {
            Layout.alignment: Qt.AlignVCenter
            text: network_status
            width: 500
            Layout.preferredWidth: width
            elide: Text.ElideRight
            color: delegate.color
        }

        Label {
            Layout.alignment: Qt.AlignVCenter
            text: duration
            width: contentWidth
            Layout.preferredWidth: width
            Layout.minimumWidth: contentWidth
            //elide: Text.ElideRight
            color: delegate.color
        }

        Label {
            Layout.alignment: Qt.AlignVCenter
            text: peerAddress
            width: contentWidth
            Layout.preferredWidth: width
            //elide: Text.ElideRight
            color: delegate.color
        }

        Label {
            Layout.alignment: Qt.AlignVCenter
            text: host
            width: 100
            Layout.preferredWidth: width
            elide: Text.ElideRight
            color: delegate.color
        }

        Label {
            Layout.alignment: Qt.AlignVCenter
            text: operation
            width: 80
            Layout.preferredWidth: width
            elide: Text.ElideRight
            color: delegate.color
        }

        Label {
            Layout.alignment: Qt.AlignVCenter
            text: url
            width: 250
            Layout.preferredWidth: width
            elide: Text.ElideRight
            color: delegate.color
        }
    }

    Rectangle {
        id: separatorBottom
        width: parent.width
        height: 1
        anchors { left: parent.left; bottom: parent.bottom }
        color: theme.separatorColor
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: delegate.ListView.view.selectRequest(index)
    }
}
