import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// ChatRoomPage.qml
Page {
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
                    text: "当前聊天室 ID:"
                }

                TextEdit {
                    id: idText
                    text: "N/A"

                    wrapMode: Text.Wrap
                    width: parent.width * 0.95
                    selectByMouse: true
                    readOnly: true
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

                    delegate: Rectangle {
                        id: msgRect
                        property bool isSender: chatroom.get_username() == username
                        
                        anchors.right: isSender ? parent.right : undefined
                        anchors.left: isSender ? undefined : parent.left
                        width: Math.min(Math.max(
                            usernameText.implicitWidth + 24,
                            messageText.implicitWidth + 24,
                            timeText.implicitWidth + 24,
                        ), messageList.width * 0.48)
                        
                        color: isSender ? "#dcf8c6" : "#888888"
                        radius: 8
                        height: usernameText.height + messageText.height + timeText.height + 24
                        layer.enabled: true

                        //  用户名
                        Text {
                            id: usernameText
                            anchors {
                                top: parent.top
                                right: msgRect.isSender ? parent.right : undefined
                                left: msgRect.isSender ? undefined : parent.left
                                topMargin: 8
                                margins: 12
                                bottomMargin: 4
                            }
                            width: parent.width
                            text: username
                            font.bold: true
                            color: isSender ? "#075e54" : "#000"
                            horizontalAlignment: isSender ? Text.AlignRight : Text.AlignLeft
                        }
                        
                        // 消息内容
                        Text {
                            id: messageText

                            //  锚点
                            anchors {
                                top: usernameText.bottom
                                right: msgRect.isSender ? parent.right : undefined
                                left: msgRect.isSender ? undefined : parent.left
                                topMargin: 4
                                leftMargin: 12
                                rightMargin: 12
                                bottomMargin: 4
                            }

                            //  最大宽度为消息气泡的 80%, 防止内容溢出
                            width: parent.width * 0.8
                            wrapMode: Text.Wrap

                            text: `${message}`
                            horizontalAlignment: msgRect.isSender ? Text.AlignRight : Text.AlignLeft
                            color: msgRect.isSender ? "black" : "white"
                        }
                        
                        // 时间戳
                        Text {
                            id: timeText
                            text: timestamp
                            anchors {
                                top: messageText.bottom
                                topMargin: 4
                                right: msgRect.isSender ? parent.right : undefined
                                left: msgRect.isSender ? undefined : parent.left
                                margins: 12
                                bottomMargin: 8
                            }
                            font.pixelSize: 10
                            color: "#777"
                        }
                    }

                    //  有新消息到达时滚动到底部
                    onCountChanged: positionViewAtEnd()
                }
            }

            // 分割线（水平线）
            Rectangle {
                Layout.fillWidth: true
                implicitHeight: 1
                color: "#e0e0e0" // 颜色可自定义
            }

            // 输入区域
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: parent.height * 0.2

                //  输入框
                TextArea {
                    id: messageInput

                    //  布局覆盖下半部分且文字超出会回环
                    anchors.fill: parent
                    wrapMode: Text.Wrap

                    //  监听回车消息用于快速发送聊天消息
                    Keys.onReturnPressed: sendButton.clicked()
                    Keys.onEnterPressed: sendButton.clicked()

                    placeholderText: "输入消息..."
                }

                Button {
                    id: sendButton
                    text: "发送"

                    //  焦点处于按钮
                    focus: true

                    //  按钮处于输入框右下角
                    anchors {
                        right: parent.right
                        bottom: parent.bottom
                        margins: 10
                    }

                    onClicked: {
                        if (messageInput.text == "") {
                            console.log("输入栏内容为空, 停止发送消息.")
                            return
                        }

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
            idText.text = chatroom.get_id() 
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