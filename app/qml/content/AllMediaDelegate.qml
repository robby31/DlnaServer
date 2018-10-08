import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import MyComponents 1.0

ListViewDelegate {
    id: delegate
    width: parent ? parent.width : 0
    height: 60

    onDoubleClicked: playMedia("file:///%1".arg(filename))

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
                Layout.alignment: Qt.AlignVCenter
                fillMode: Image.PreserveAspectFit
                sourceSize.height: delegate.height-10
                source: "image://media/" + picture
                asynchronous: false
                clip: true
            }

            Text {
                text: mediaId
                Layout.preferredWidth: 50
                Layout.alignment: Qt.AlignVCenter
                elide: Text.ElideRight
                clip: true
            }

            Image {
                Layout.alignment: Qt.AlignVCenter
                fillMode: Image.PreserveAspectFit
                sourceSize.height: parent.height-10
                source: "image://format/%1/%2".arg(mediaType).arg(format)
                asynchronous: false
                clip: true
            }

            Text {
                text: format
                Layout.preferredWidth: 50
                Layout.alignment: Qt.AlignVCenter
                elide: Text.ElideRight
                clip: true
            }

            ModelEditableText {
                text: title
                placeholderText: "unknown title"
                Layout.preferredWidth: 400
                Layout.alignment: Qt.AlignVCenter
                onEditingFinished: {
                    title = text
                    focus = false
                }
                clip: true
            }

            ModelEditableComboBox {
                Layout.preferredWidth: 200
                Layout.alignment: Qt.AlignVCenter
                editable: true
                clip: true

                placeholderText: "unknown artist"

                isCurrentItem: delegate.ListView.isCurrentItem
                modelText: artistName
                model: artistModel
                textRole: "name"

                onUpdateModelText: {
                    var newIndex = model.findRow(text, "name")
                    var obj = model.get(newIndex)
                    if (obj.id !== undefined)
                    {
                        // add entry existing in combo model
                        artist = obj.id
                    }
                    else
                    {
                        // add entry in combo model
                        var artist_id = model.append({name: text})
                        if (artist_id === -1)
                        {
                            console.log("unable to add new artist", text, artist_id)
                        }
                        else
                        {
                            model.reload()
                            artist = artist_id
                        }
                    }
                }
            }

            ModelEditableComboBox {
                Layout.preferredWidth: 200
                Layout.alignment: Qt.AlignVCenter
                editable: true
                clip: true

                placeholderText: "unknown album"

                isCurrentItem: delegate.ListView.isCurrentItem
                modelText: albumName
                model: albumModel
                textRole: "name"

                onUpdateModelText: {
                    // text written by keyboard
                    var newIndex = model.findRow(text, "name")
                    var obj = model.get(newIndex)
                    if (obj.id !== undefined)
                    {
                        // add entry existing in combo model
                        album = obj.id
                    }
                    else
                    {
                        // add entry in combo model
                        var album_id = model.append({name: text})
                        if (album_id === -1)
                        {
                            console.log("unable to add new album", text, album_id)
                        }
                        else
                        {
                            model.reload()
                            album = album_id
                        }
                    }
                }
            }

            ModelEditableComboBox {
                Layout.preferredWidth: 150
                Layout.alignment: Qt.AlignVCenter
                editable: true
                clip: true

                placeholderText: "unknown genre"

                isCurrentItem: delegate.ListView.isCurrentItem
                modelText: genreName
                model: genreModel
                textRole: "name"

                onUpdateModelText: {
                    // text written by keyboard
                    var newIndex = model.findRow(text, "name")
                    var obj = model.get(newIndex)
                    if (obj.id !== undefined)
                    {
                        // add entry existing in combo model
                        genre = obj.id
                    }
                    else
                    {
                        // add entry in combo model
                        var genre_id = model.append({name: text})
                        if (genre_id === -1)
                        {
                            console.log("unable to add new genre", text, genre_id)
                        }
                        else
                        {
                            model.reload()
                            genre = genre_id
                        }
                    }
                }
            }

            Text {
                text: filename
                Layout.preferredWidth: 200
                Layout.alignment: Qt.AlignVCenter
                elide: Text.ElideRight
                clip: true
            }
        }
    }
}
