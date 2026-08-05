import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: window
    width: 420
    height: 750
    visible: true
    title: "Smart Home"

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: loginPageComponent
    }

    // ---------- تعريف الصفحات كـ Components ----------
    // بنستخدم Signals عشان كل صفحة تقول لل Main.qml "خلصت، انتقل للصفحة اللي بعدي"
    // ده أضمن وأوضح من إن كل صفحة تتحكم في التنقل بنفسها

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
