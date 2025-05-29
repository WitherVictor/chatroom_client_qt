import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs

Page {
    id: waitConnectionPage

    RowLayout {
        anchors.centerIn: parent
        spacing: 25

        BusyIndicator {
            running: true
        }

        Label {
            id: label
            text: "正在连接到服务器..."
        }
    }

    MessageDialog {
        id: disconnectedDialog

        text: "服务器连接已断开!"
        buttons: MessageDialog.Ok

        onAccepted: function() {
            stackView.pop(null)
            stackView.push("WaitConnection.qml")
            animateResizeAndCenter(480, 270)
            network.try_connect_to_server()
        }
    }

    Connections {
        target: network

        function onConnected() {
            console.log("QML 切换到登录界面。")
            stackView.push("Login.qml")
        }

        function onDisconnected() {
            console.log("返回正在连接页面。")

            disconnectedDialog.open();
        }
    }

    Component.onCompleted: {
        console.log("界面加载完成，准备连接服务器。")
        network.try_connect_to_server()
    }
}