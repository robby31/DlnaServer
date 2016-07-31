import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.1
import MyComponents 1.0
import myTypes 1.0

Page {
    id: libraryView

    width: 600
    height: 300

    signal quit()

    actions: pageActions

    onActionClicked: {
        if (name == "Quit")
            quit()
        else if (name == "Reload")
        {
            mediaModel.reload()
        }
    }

    ListModel {
        id: pageActions

        ListElement {
            name: "Reload"
            description: "reload library"
            icon: "qrc:///images/exit.png"
        }

        ListElement {
            name: "Quit"
            description: "exit application"
            icon: "qrc:///images/exit.png"
        }
    }

    Component {
        id: backgroundTextSelected
        Rectangle {
            radius: 2
            implicitWidth: 100
            implicitHeight: 24
            border.color: "#333"
            border.width: 1
        }
    }

    Component {
        id: backgroundText
        Item  {

        }
    }

    Component {
        id: mediaDelegate

        Item {
            id: delegate
            width: parent.width
            height: 40

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

            Rectangle {
                id: selected
                anchors.fill: parent
                color: theme.highlightSelectColor
                visible: delegate.ListView.isCurrentItem
            }

            Row {
                width: parent.width-10
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                spacing: 5

                MyButton {
                    id: button

                    width: 20*1.5
                    height: 20*1.5
                    hovered: true
                    visible: delegate.ListView.isCurrentItem

                    sourceComponent: Image {
                        id: remove
                        z: 1
                        width: 20
                        height: width
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: "qrc:///images/clear.png"
                    }

                    onButtonClicked:  {
                        _app.removeMedia(model["id"])
                        mediaModel.reload()
                    }
                }

                Text {
                    id: textId
                    text: model["id"]
                    width: 100
                    height: contentHeight
                    anchors.verticalCenter: parent.verticalCenter
                    clip: true
                }

                TextField {
                    id: textFilename
                    text: model["filename"]
                    width: (parent.width-button.width-textId.width)/2
                    anchors.verticalCenter: parent.verticalCenter
                    clip: true
                    readOnly: !delegate.ListView.isCurrentItem
                    style: TextFieldStyle {
                              textColor: "black"
                              background: delegate.ListView.isCurrentItem ? backgroundTextSelected : backgroundText
                          }

                    onAccepted: _app.updateFilenameMedia(id, text)
                }

                Text {
                    id: textTitle
                    text: model["title"]
                    width: (parent.width-button.width-textId.width)/2
                    anchors.verticalCenter: parent.verticalCenter
                    elide: Text.ElideRight
                    clip: true
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
                onClicked: listView.currentIndex = index
                visible: !delegate.ListView.isCurrentItem
            }
        }        
    }

    SqlListModel {
        id: mediaModel
        connectionName: "MEDIA_DATABASE"
        tablename: "media"
        query: "SELECT id, filename, title from media where is_reachable=0"
    }

    ScrollView
    {
        anchors.fill: parent

        ListView {
            id: listView
            anchors.fill: parent
            anchors.margins: 5
            clip: true
            model: mediaModel
            delegate: mediaDelegate
            antialiasing: true
        }
    }
}
