import QtQuick 2.5
import MyComponents 1.0
import QtQuick.Dialogs 1.2


MyApplication {
    id: mainWindow
    title: "QT Player"
    //logoCompany: "qrc:///images/icon-32.png"
    minimumWidth: 900
    width: minimumWidth

    controller: homePageController
    modelButtons : mybuttons
    srcPages: "Pages/ApplicationPages.qml"

    function chooseDatabase() {
        chooseDatabaseDialog.open()
    }

    ListModel {
        id: mybuttons

        ListElement {
            title: "Servers"
            state: "SERVERS"
            icon: "qrc:/images/server.png"
        }
    }
}
