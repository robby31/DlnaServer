import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import MyComponents 1.0

ListViewDelegate {
    id: delegate
    width: parent ? parent.width : 0
    height: 60

    contentItem: Item {
        width: parent.width
        height: delegate.height

        MouseArea {
            anchors.fill: parent
            onDoubleClicked: playMedia("file:///%1".arg(filename))
        }

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
                source: "image://media/" + picture
                asynchronous: false
                clip: true
            }

            Text {
                text: mediaId
                Layout.preferredWidth: 50
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideRight
                clip: true
            }

            Image {
                anchors.verticalCenter: parent.verticalCenter
                fillMode: Image.PreserveAspectFit
                sourceSize.height: parent.height-10
                source: "image://format/%1/%2".arg(mediaType).arg(format)
                asynchronous: false
                clip: true
            }

            Text {
                text: format
                Layout.preferredWidth: 50
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideRight
                clip: true
            }

            EditableText {
                text: title
                placeholderText: "unknown title"
                Layout.preferredWidth: 400
                anchors.verticalCenter: parent.verticalCenter
                onEditingFinished: {
                    title = text
                    focus = false
                }
                clip: true
            }

            EditableComboBox {
                Layout.preferredWidth: 200
                anchors.verticalCenter: parent.verticalCenter

                placeholderText: "unknown artist"

                model: artistModel
                textRole: "name"
                editable: true
                clip: true

                onTextUpdated: {
                    var obj = model.get(currentIndex)
                    if (obj.id !== undefined)
                    {
                        artist = obj.id
                        focus = false
                    }
                    else
                    {
                        console.log("unable to update artist", currentText, obj.id)
                    }
                }

                onAccepted: {
                    var newIndex = artistModel.findRow(editText, "name")
                    var obj = artistModel.get(newIndex)
                    if (obj.id !== undefined)
                    {
                        artist = obj.id
                        currentIndex = newIndex
                        focus = false
                    }
                    else
                    {
                        var artist_id = artistModel.append({name: editText})
                        if (artist_id === -1)
                        {
                            console.log("unable to update artist", editText, artist_id)
                            canceled()
                        }
                        else
                        {
                            artistModel.reload()
                            artist = artist_id
                            currentIndex = model.findRow(editText, "name")
                            focus = false
                        }
                    }
                }

                Component.onCompleted: currentIndex = model.findRow(artistName, "name")
            }

            EditableComboBox {
                Layout.preferredWidth: 200
                anchors.verticalCenter: parent.verticalCenter

                placeholderText: "unknown album"

                model: albumModel
                textRole: "name"
                editable: true
                clip: true


                onTextUpdated: {
                    var obj = albumModel.get(currentIndex)
                    if (obj.id !== undefined)
                    {
                        album = obj.id
                        focus = false
                    }
                    else
                    {
                        console.log("unable to update album", currentText, obj.id)
                    }
                }

                onAccepted: {
                    var newIndex = albumModel.findRow(editText, "name")
                    var obj = albumModel.get(newIndex)
                    if (obj.id !== undefined)
                    {
                        album = obj.id
                        currentIndex = newIndex
                        focus = false
                    }
                    else
                    {
                        var album_id = albumModel.append({name: editText})
                        if (album_id === -1)
                        {
                            console.log("unable to update album", editText, album_id)
                            canceled()
                        }
                        else
                        {
                            albumModel.reload()
                            album = album_id
                            currentIndex = model.findRow(editText, "name")
                            focus = false
                        }
                    }
                }

                Component.onCompleted: currentIndex = model.findRow(albumName, "name")
            }

            EditableComboBox {
                Layout.preferredWidth: 150
                anchors.verticalCenter: parent.verticalCenter

                placeholderText: "unknown genre"

                model: genreModel
                textRole: "name"
                editable: true
                clip: true


                onTextUpdated: {
                    var obj = genreModel.get(currentIndex)
                    if (obj.id !== undefined)
                    {
                        genre = obj.id
                        focus = false
                    }
                    else
                    {
                        console.log("unable to update genre", currentText, obj.id)
                    }
                }

                onAccepted: {
                    var newIndex = genreModel.findRow(editText, "name")
                    var obj = genreModel.get(newIndex)
                    if (obj.id !== undefined)
                    {
                        genre = obj.id
                        currentIndex = newIndex
                        focus = false
                    }
                    else
                    {
                        var genre_id = genreModel.append({name: editText})
                        if (genre_id === -1)
                        {
                            console.log("unable to update genre", editText, genre_id)
                            canceled()
                        }
                        else
                        {
                            genreModel.reload()
                            genre = genre_id
                            currentIndex = model.findRow(editText, "name")
                            focus = false
                        }
                    }
                }

                Component.onCompleted: currentIndex = model.findRow(genreName, "name")
            }

            Text {
                text: filename
                Layout.preferredWidth: 200
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideRight
                clip: true
            }
        }
    }
}
