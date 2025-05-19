import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: registerPage
    property var loader

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
            
            Button {
                text: "注册"
                onClicked: console.log("检测到注册按钮被点击")
            }
            Button {
                text: "返回"
                onClicked: {
                    console.log("返回按钮被点击，跳转到登录界面。")
                    stackView.pop()
                }
            }
        }
    }
}