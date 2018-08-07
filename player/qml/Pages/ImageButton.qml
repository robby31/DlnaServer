import QtQuick 2.0
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0

Button {
    id: control
    width: height
    height: 300

    property alias source: image.source

    contentItem: Image {
        id: image
        fillMode: Image.PreserveAspectFit
        opacity: control.enabled ? 1.0 : 0.3
    }

    background: Item { }

    Colorize {
        anchors.fill: control
        source: control
        hue: 0.0
        saturation: 0.5
        lightness: 1.0
        visible: control.down
    }
}
