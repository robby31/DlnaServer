import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import MyComponents 1.0

ListViewDelegate {
    id: delegate
    width: parent ? parent.width : 0
    height: 60

    contentItem: Item {
        width: parent.width
        height: delegate.height

        RowLayout {
            width: parent.width-10
            height: parent.height
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            spacing: 5

            Image {
                anchors.verticalCenter: parent.verticalCenter
                fillMode: Image.PreserveAspectFit
                sourceSize.height: delegate.height-10
                source: "image://media/" + model["picture"]
                asynchronous: false
                clip: true
            }

            Text {
                text: model["id"]
                Layout.preferredWidth: 50
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideRight
                clip: true
            }

            Image {
                anchors.verticalCenter: parent.verticalCenter
                fillMode: Image.PreserveAspectFit
                sourceSize.height: parent.height-10
                source: "image://format/%1/%2".arg(model["mediaType"]).arg(model["format"])
                asynchronous: false
                clip: true
            }

            Text {
                text: model["format"]
                Layout.preferredWidth: 50
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideRight
                clip: true
            }

            EditableText {
                text: model["title"]
                placeholderText: "unknown title"
                Layout.preferredWidth: 400
                anchors.verticalCenter: parent.verticalCenter
                onEditingFinished: model["title"] = text
                clip: true
            }

            EditableText {
                text: model["artistName"]
                placeholderText: "unknown artist"
                Layout.preferredWidth: 200
                anchors.verticalCenter: parent.verticalCenter
                onEditingFinished: {
                    var artistId = artistModel.getArtistId(text)
                    if (artistId !== -1)
                        model["artist"] = artistId
                    else
                        console.log("unable to update artist", text, artistId)
                }
                clip: true
            }

            EditableText {
                text: model["albumName"]
                placeholderText: "unknown album"
                Layout.preferredWidth: 200
                anchors.verticalCenter: parent.verticalCenter
//                onEditingFinished: model["albumName"] = text
                clip: true
            }

            Text {
                text: model["filename"]
                Layout.fillWidth: true
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideRight
                clip: true
            }
        }
    }
}
