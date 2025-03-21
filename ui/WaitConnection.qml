import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Page {
    id: waitConnectionPage
    property var loader

    RowLayout {
        anchors.centerIn: parent
        spacing: 25

        AnimatedImage {
            id: loadingGif
            source: "../icon/loading-50px.gif"
        }

        Label {
            id: label
            text: "正在连接到服务器..."
        }
    }

    Timer {
        id: timer
        interval: 2000
        onTriggered: {
            waitConnectionPage.loader.source = "ui/Login.qml"
        }
    }

    Connections {
        target: network

        function onConnectSuccess() {
            timer.start()
        }
    }

    Component.onCompleted: {
        console.log("界面加载完成，准备连接服务器。")
        network.try_connect_to_server()
    }
}