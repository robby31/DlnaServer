import QtQuick 2.5
import MyComponents 1.0
import "../content"

Item {
    id: pages

    function toggleMenu() {
        if (mainWindow.state==="REQUESTS")
            requestsPage.toggleMenu()
        else if (mainWindow.state==="RENDERERS")
            renderersPage.toggleMenu()
        else if (mainWindow.state==="SHARED")
            sharedPage.toggleMenu()
        else if (mainWindow.state==="NAVIGATOR")
            webPage.toggleMenu()
        else if (mainWindow.state==="DEBUG")
            debugPage.toggleMenu()
        else if (mainWindow.state==="LIBRARY")
            libraryPage.toggleMenu()
    }

    RequestsView {
        id: requestsPage
        anchors.fill: parent
        visible: mainWindow.state === "REQUESTS"
        onQuit: mainWindow.close()
    }

    RenderersView {
        id: renderersPage
        anchors.fill: parent
        visible: mainWindow.state === "RENDERERS"
        onQuit: mainWindow.close()
    }

    SharedView {
        id: sharedPage
        anchors.fill: parent
        visible: mainWindow.state === "SHARED"
        onQuit: mainWindow.close()
    }

    Navigator {
        id: webPage
        anchors.fill: parent
        visible: mainWindow.state === "NAVIGATOR"
        onQuit: mainWindow.close()
    }

    DebugView {
        id: debugPage
        anchors.fill: parent
        visible: mainWindow.state === "DEBUG"
        onQuit: mainWindow.close()
    }

    LibraryView {
        id: libraryPage
        anchors.fill: parent
        visible: mainWindow.state === "LIBRARY"
        onQuit: mainWindow.close()
    }
}
