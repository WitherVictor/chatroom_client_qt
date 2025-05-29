import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Page {
    id: loginPage
    property alias username: usernameField.text
    property alias password: passwordField.text

    //  监听回车事件
    Keys.onReturnPressed: loginButton.clicked()
    Keys.onEnterPressed: loginButton.clicked()

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

            //  用户名或密码为空弹出的提示框
            MessageDialog {
                id: loginDialog

                //  只有一个确定按钮
                buttons: MessageDialog.Ok
            }
            
            Button {
                id: loginButton
                text: "登录"
                onClicked: {
                    if (loginPage.username == "") {
                        loginDialog.text = "用户名不能为空!"
                        loginDialog.open()

                        return
                    }

                    if (loginPage.password == "") {
                        loginDialog.text = "不能为空!"
                        loginDialog.open()
                        
                        return
                    }

                    console.log("尝试登录到服务器，用户名: " + loginPage.username, + ", 密码: " + loginPage.password + ".")
                    login_handler.try_login(loginPage.username, loginPage.password)

                    focus: true
                }
            }
            Button {
                text: "注册"
                onClicked: {
                    console.log("注册按钮被点击，跳转到注册界面。")
                    stackView.push("Signup.qml")
                }
            }
        }
    }

    Connections {
        target: login_handler

        function onLoginSuccess() {
            console.log("登录成功")
            chatroom.set_username(loginPage.username)
            stackView.push("Chatroom.qml")
        }

        function onLoginFailed() {
            console.log("登录失败")
        }
    }
}