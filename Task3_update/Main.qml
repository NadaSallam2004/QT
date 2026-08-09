import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: window
    width: 420
    height: 750
    visible: true
    title: qsTr("Smart Home")
    LayoutMirroring.enabled: languageManager.isRtl
    LayoutMirroring.childrenInherit: true

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: loginPageComponent
    }

    Component {
        id: loginPageComponent
        LoginPage {
            onLoginSucceeded: {
                console.log("Main.qml received loginSucceeded signal")
                stackView.push(dashboardPageComponent)
            }
        }
    }

    Component {
        id: dashboardPageComponent
        DashboardPage {
            onOpenSettingsRequested: {
                stackView.push(settingsPageComponent)
            }
        }
    }

    Component {
        id: settingsPageComponent
        SettingsPage {
            onBackRequested: {
                stackView.pop()
            }
        }
    }
}
