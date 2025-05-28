import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// ChatRoomPage.qml
Page {
    width: Screen.width * 0.75
    height: Screen.height * 0.75

    RowLayout {
        anchors.fill: parent
        spacing: 0 // 取消间距

        // 左边功能栏 (固定宽度)
        Rectangle {
            Layout.preferredWidth: 200
            Layout.fillHeight: true
            color: "#f5f5f5"

            Column {
                anchors.fill: parent
                padding: 10

                Text {
                    id: idText
                    text: "当前聊天室 ID: 无"
                }

                Button {
                    text: "创建群组"
                    width: parent.width - 20
                    onClicked: {
                        console.log("尝试创建新的聊天室.")
                        chatroom.create_chatroom()
                    }
                }

                Button {
                    text: "加入群组"
                    width: parent.width - 20
                }

                // 更多功能...
            }

            Rectangle { // 右侧分割线
                width: 1
                anchors.right: parent.right
                height: parent.height
                color: "#ddd"
            }
        }

        // 中间聊天区 (自动扩展)
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            // 消息显示区域
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ListView {
                    id: messageList
                    model: ListModel { id: listModel } // 消息数据模型
                    spacing: 8
                    verticalLayoutDirection: ListView.BottomToTop

                    delegate: Text {
                        text: `${timestamp}\n${username}: ${message}`
                    }
                }
            }

            // 分割线（水平线）
            Rectangle {
                Layout.fillWidth: true
                implicitHeight: 1
                color: "#e0e0e0" // 颜色可自定义
            }

            // 输入区域
            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: parent.height * 0.2
                spacing: 10

                TextField {
                    id: messageInput

                    Layout.fillWidth: true
                    Layout.preferredHeight: parent.height * 0.6

                    placeholderText: "输入消息..."
                }

                Button {
                    text: "发送"

                    onClicked: {
                        console.log("准备向服务器发送聊天消息.")
                        chatroom.send_message(messageInput.text)
                        messageInput.text = ""
                    }
                }
            }
        }

        // 右边用户列表 (固定宽度)
        Rectangle {
            Layout.preferredWidth: 180
            Layout.fillHeight: true
            color: "#fafafa"

            Column {
                anchors.fill: parent

                Label {
                    text: "在线用户 (" + "TESTCOUNT" + ")"
                    padding: 10
                }

                ListView {
                    width: parent.width
                    height: parent.height - 40
                    model: ListModel { // 用户数据模型
                        ListElement { name: "张三"; avatar: "user1.png" }
                        ListElement { name: "李四"; avatar: "user2.png" }
                    }
                }
            }

            Rectangle { // 左侧分割线
                width: 1
                anchors.left: parent.left
                height: parent.height
                color: "#ddd"
            }
        }
    }

    Connections {
        target: chatroom

        function onChatroomIdReady() {
            idText.text = "当前聊天室 ID: " + chatroom.get_id() 
        }

        function onNewMessage(timestamp, username, message) {
            listModel.append({
                timestamp: timestamp,
                username: username,
                message: message
            })

            messageList.positionViewAtEnd()
        }
    }
}