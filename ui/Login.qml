import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: loginPage
    property alias username: usernameField.text
    property alias password: passwordField.text

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
                id: usernameField
                placeholderText: "请输入您的用户名"
            }

            //  密码段
            Label { text: "密码" }
            TextField {
                id: passwordField
                echoMode: TextInput.Password
                placeholderText: "请输入密码"
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            
            Button {
                text: "登录"
                onClicked: {
                    console.log("尝试登录到服务器，用户名：{}，密码：{}", loginPage.username, loginPage.password)
                    login_handler.try_login(loginPage.username, loginPage.password)
                }
            }
            Button {
                text: "注册"
                onClicked: {
                    console.log("注册按钮被点击，跳转到注册界面。")
                    stackView.push("Register.qml")
                }
            }
        }
    }

    Connections {
        target: login_handler

        function onLoginSuccess() {
            console.log("登录成功")
        }

        function onLoginFailed() {
            console.log("登录失败")
        }
    }
}