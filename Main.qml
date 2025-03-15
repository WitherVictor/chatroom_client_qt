import QtQuick
import QtQuick.Controls

ApplicationWindow {
    width: 480
    height: 360
    visible: true
    title: qsTr("猫猫聊天室")
    color: "lightgrey"

    Loader {
        id: loader
        anchors.fill: parent
        source: "ui/Login.qml"

        onLoaded: {
            if (item.hasOwnProperty("loader")) {
                item.loader = loader
            }
        }
    }
}
