import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import MyComponents 1.0

ListViewDelegate {
    id: delegate
    height: 100

    onDoubleClicked: playMedia("file:///%1".arg(filename))

    contentItem: Item {
        id: item
        width: parent.width-10
        height: parent.height-10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        RowLayout {
            anchors.fill: parent
            spacing: 5

            Image {
                Layout.alignment: Qt.AlignVCenter
                fillMode: Image.PreserveAspectFit
                sourceSize.height: parent.height
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
                sourceSize.width: 40
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
                    var artist_index = find(text)
                    if (artist_index !== -1)
                    {
                        // add existing artist in combo model
                        var obj = model.get(artist_index)
                        artist = obj.id
                    }
                    else
                    {
                        model.append({name: text})
                        model.select()
                        artist_index = find(text)
                        if (artist_index !== -1)
                        {
                            obj = model.get(artist_index)
                            artist = obj.id
                        }
                        else
                        {
                            console.log("unable to find artist", text, "in model", model)
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
                    var album_index = find(text)
                    if (album_index !== -1)
                    {
                        // add existing album in combo model
                        var obj = model.get(album_index)
                        album = obj.id
                    }
                    else
                    {
                        model.append({name: text})
                        model.select()
                        album_index = find(text)
                        if (album_index !== -1)
                        {
                            obj = model.get(album_index)
                            album = obj.id
                        }
                        else
                        {
                            console.log("unable to find album", text, "in model", model)
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
                    var genre_index = find(text)
                    if (genre_index !== -1)
                    {
                        // add existing genre in combo model
                        var obj = model.get(genre_index)
                        genre = obj.id
                    }
                    else
                    {
                        model.append({name: text})
                        model.select()
                        genre_index = find(text)
                        if (genre_index !== -1)
                        {
                            obj = model.get(genre_index)
                            genre = obj.id
                        }
                        else
                        {
                            console.log("unable to find genre", text, "in model", model)
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
