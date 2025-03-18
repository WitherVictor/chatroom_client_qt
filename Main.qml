import QtQuick
import QtQuick.Controls

ApplicationWindow {
    width: 480
    height: 360
    visible: true
    title: qsTr("猫猫聊天室")

    Loader {
        id: loader
        anchors.fill: parent
        source: "ui/WaitConnection.qml"

        onLoaded: {
            if (item.hasOwnProperty("loader")) {
                item.loader = loader
            }
        }
    }
}
