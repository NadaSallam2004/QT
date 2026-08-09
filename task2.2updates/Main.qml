import QtQuick
import QtQuick.Controls

ApplicationWindow {
    width: 480
    height: 600
    visible: true
    title: "My App"

    Rectangle {
        id: splashScreen
        anchors.fill: parent
        color: "white"
        z: 10

        Image {
            id: logoImage
            anchors.centerIn: parent
            width: 200
            height: 200
            fillMode: Image.PreserveAspectFit
            asynchronous: true
            source: "https://picsum.photos/200"
        }

        BusyIndicator {
            anchors.centerIn: parent
            running: logoImage.status === Image.Loading
        }

        Text {
            anchors.centerIn: parent
            text: "Failed to load image"
            color: "red"
            visible: logoImage.status === Image.Error
        }

        Timer {
            interval: 3000
            running: true
            repeat: false
            onTriggered: {
                splashScreen.visible = false
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#f7fafc"

        Text {
            anchors.centerIn: parent
            text: "Welcome to the App!"
            font.pixelSize: 24
            color: "#1a202c"
        }
    }
}