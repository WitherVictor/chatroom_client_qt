import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Page {
    id: registerPage

    //  监听回车事件
    Keys.onReturnPressed: registerButton.clicked()
    Keys.onEnterPressed: registerButton.clicked()

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 25
        
        Label {
            text: "注册账户"

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
                id: tryRegisterDialog

                //  只有一个确定按钮
                buttons: MessageDialog.Ok
            }
            
            Button {
                id: registerButton
                text: "注册"
                onClicked: function() {
                    //  用户名或密码为空时, 禁止注册
                    if (usernameField.text == "") {
                        tryRegisterDialog.text = "用户名不能为空!"
                        tryRegisterDialog.open()

                        return
                    }

                    if (passwordField.text == "") {
                        tryRegisterDialog.text = "密码不能为空!"
                        tryRegisterDialog.open()
                        
                        return
                    }

                    console.debug("向后端传递登录信息.")
                    signup.process_request(usernameField.text, passwordField.text)
                }
            }
            Button {
                text: "返回"
                onClicked: {
                    console.debug("返回按钮被点击，跳转到登录界面。")
                    stackView.pop()
                }
            }
        }
    }

    MessageDialog {
        id: registerDialog

        text: ""
        buttons: MessageDialog.Ok

        onAccepted: function() {
            stackView.pop()
        }
    }

    Connections {
        target: signup
        
        function onRegisterSuccess() {
            console.debug("接收到登陆成功信号, 弹出窗口.")

            registerDialog.text = "注册成功! 请返回登录页面登录."
            registerDialog.open()
        }

        function onRegisterFailed() {
            console.debug("接收到注册失败信号! 弹出窗口并展示错误信息")

            registerDialog.text = "注册失败!"
            registerDialog.open()
        }
    }
}