import QtQuick.Controls

ApplicationWindow {
    id: root

    width: 480
    height: 360
    visible: true
    title: qsTr("猫猫聊天室")

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: "ui/WaitConnection.qml"
    }
}
