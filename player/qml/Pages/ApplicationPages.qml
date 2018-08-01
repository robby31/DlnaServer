import QtQuick 2.5
import MyComponents 1.0

Item {
    id: pages

    width: 600
    height: 300

    function toggleMenu() {
        if (mainWindow.state==="SERVERS")
            serversPage.toggleMenu()
        else if (mainWindow.state==="PLAYER")
            playerPage.toggleMenu()
    }

    ServersPage {
        id: serversPage
        anchors.fill: parent
        visible: mainWindow.state === "SERVERS"
        onQuit: mainWindow.close()
    }

    PlayerPage {
        id: playerPage
        anchors.fill: parent
        visible: mainWindow.state === "PLAYER"
        onQuit: mainWindow.close()
    }

    function playMedia(objectData) {
        playerPage.objectData = objectData
    }
}
