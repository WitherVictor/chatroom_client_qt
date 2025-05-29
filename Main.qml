import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: root

    width: 480
    height: 270
    visible: true
    title: qsTr("猫猫聊天室")

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: "ui/WaitConnection.qml"
    }

    // 居中窗口函数
    function centerWindow() {
        if (!Screen || !Screen.desktopAvailableWidth) return;
        
        // 计算居中位置
        const screenWidth = Screen.desktopAvailableWidth;
        const screenHeight = Screen.desktopAvailableHeight;
        
        root.x = (screenWidth - width) / 2;
        root.y = (screenHeight - height) / 2;
    }

    // 带动画调整大小并居中
    function animateResizeAndCenter(width, height) {
        // 计算目标位置（居中）
        const screenWidth = Screen.desktopAvailableWidth;
        const screenHeight = Screen.desktopAvailableHeight;
        const targetX = (screenWidth - width) / 2;
        const targetY = (screenHeight - height) / 2;
        
        // 启动动画
        resizeAnim.widthTo = width;
        resizeAnim.heightTo = height;
        resizeAnim.xTo = targetX;
        resizeAnim.yTo = targetY;
        resizeAnim.start();
    }
    
    // 平滑的尺寸和位置变化动画
    ParallelAnimation {
        id: resizeAnim
        property real widthTo: root.width
        property real heightTo: root.height
        property real xTo: root.x
        property real yTo: root.y
        
        NumberAnimation {
            target: root
            property: "width"
            to: resizeAnim.widthTo
            duration: 350
            easing.type: Easing.OutCubic
        }
        
        NumberAnimation {
            target: root
            property: "height"
            to: resizeAnim.heightTo
            duration: 350
            easing.type: Easing.OutCubic
        }
        
        NumberAnimation {
            target: root
            property: "x"
            to: resizeAnim.xTo
            duration: 350
            easing.type: Easing.OutCubic
        }
        
        NumberAnimation {
            target: root
            property: "y"
            to: resizeAnim.yTo
            duration: 350
            easing.type: Easing.OutCubic
        }
    }
}
