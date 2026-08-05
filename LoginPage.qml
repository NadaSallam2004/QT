import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: loginPage

    signal loginSucceeded()

    readonly property color bgColor: "#FFECC6"
    readonly property color primaryColor: "#BB9351"
    readonly property color textColor: "#3D2D13"
    readonly property color errorColor: "#B87333"

    background: Rectangle { color: loginPage.bgColor }

    ColumnLayout {
        anchors.centerIn: parent
        width: parent.width * 0.82
        spacing: 18

        Image {
            Layout.alignment: Qt.AlignHCenter
            source: "images/user.svg"
            Layout.preferredWidth: 100
            Layout.preferredHeight: 100
            fillMode: Image.PreserveAspectFit
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: "Smart Home"
            font.pixelSize: 26
            font.bold: true
            color: loginPage.textColor
        }

        TextField {
            id: usernameField
            Layout.fillWidth: true
            placeholderText: "Username"
            selectByMouse: true
        }

        TextField {
            id: passwordField
            Layout.fillWidth: true
            placeholderText: "Password"
            echoMode: TextInput.Password
            selectByMouse: true
        }

        Label {
            id: errorLabel
            Layout.alignment: Qt.AlignHCenter
            text: "Please enter both username and password"
            color: loginPage.errorColor
            visible: false
        }

        Button {
            id: loginButton
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 160
            text: "Login"
            enabled: !busyIndicator.running

            background: Rectangle {
                radius: 8
                color: loginPage.primaryColor
            }
            contentItem: Label {
                text: loginButton.text
                color: "white"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: {
                console.log("Login button clicked")
                if (usernameField.text.length > 0 && passwordField.text.length > 0) {
                    errorLabel.visible = false
                    busyIndicator.running = true
                    statusLabel.text = "Loading..."
                    loginTimer.start()
                } else {
                    errorLabel.visible = true
                    console.log("Login failed: empty fields")
                }
            }
        }

        BusyIndicator {
            id: busyIndicator
            Layout.alignment: Qt.AlignHCenter
            running: false
        }

        Label {
            id: statusLabel
            Layout.alignment: Qt.AlignHCenter
            text: ""
            color: loginPage.textColor
            font.pixelSize: 13
        }
    }

    Timer {
        id: loginTimer
        interval: 2000
        repeat: false
        onTriggered: {
            console.log("Timer triggered, emitting loginSucceeded signal")
            busyIndicator.running = false
            statusLabel.text = ""
            loginPage.loginSucceeded()
        }
    }
}
