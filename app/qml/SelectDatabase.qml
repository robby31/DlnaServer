import QtQuick 2.0
import QtQuick.Controls 2.1

Item {
    anchors.margins: 10

    Column {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        spacing: 10

        TextArea {
            text: "file for database is not set.\nPlease choose a file."
            font.pointSize: 14
        }

        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Choose"
            onClicked: chooseDatabase()
        }
    }
}
