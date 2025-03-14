import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    width: 480
    height: 360
    visible: true
    title: qsTr("猫猫聊天室")

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 25
        width: parent.width * 0.8

        Label {
            text: "猫猫聊天室"

            font.pixelSize: 24
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
            color: "#333"
        }

        GridLayout {
            columns: 2
            rowSpacing: 5
            columnSpacing: 5
            Layout.alignment: Qt.AlignHCenter

            //  用户名段
            Label { text: "用户名" }
            TextField {
                placeholderText: "请输入您的用户名"
            }

            //  密码段
            Label { text: "密码" }
            TextField {
                echoMode: TextInput.Password
                placeholderText: "请输入密码"
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            
            Button { text: "登录" }
            Button { text: "注册" }
        }
    }
}
