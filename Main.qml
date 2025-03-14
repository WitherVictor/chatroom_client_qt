import QtQuick

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Chatroom")

    Text {
        text: "Hello world!"

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }
}
