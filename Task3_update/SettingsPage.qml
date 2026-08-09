import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: settingsPage
    signal backRequested()
    readonly property color bgColor: "#FFECC6"
    readonly property color headerColor: "#654321"
    readonly property color primaryColor: "#BB9351"
    readonly property color textColor: "#3D2D13"

    background: Rectangle { color: settingsPage.bgColor }

    header: ToolBar {
        background: Rectangle { color: settingsPage.headerColor }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 8
            anchors.rightMargin: 12

            Button {
                text: qsTr("< Back")
                onClicked: settingsPage.backRequested()
            }

            Label {
                text: qsTr("Settings")
                color: "white"
                font.bold: true
                font.pixelSize: 17
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }

            Item { Layout.preferredWidth: 60 }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 28

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 6

            Label {
                text: qsTr("Application Language")
                font.bold: true
                color: settingsPage.textColor
            }

            ComboBox {
                id: languageComboBox
                Layout.fillWidth: true
                model: [
                    { code: "en", label: qsTr("English") },
                    { code: "ar", label: qsTr("Arabic") }
                ]
                textRole: "label"
                valueRole: "code"
                Component.onCompleted: currentIndex = indexOfValue(languageManager.currentLanguage)
                Connections {
                    target: languageManager
                    function onLanguageChanged() {
                        languageComboBox.currentIndex = languageComboBox.indexOfValue(languageManager.currentLanguage)
                    }
                }

                onActivated: {
                    languageManager.switchLanguage(currentValue)
                }
            }
        }
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 6

            Label {
                text: qsTr("Screen Brightness")
                font.bold: true
                color: settingsPage.textColor
            }

            Slider {
                Layout.fillWidth: true
                from: 0
                to: 100
                value: 50
                onMoved: {
                    console.log("Brightness set to:", Math.round(value))
                }
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 6

            Label {
                text: qsTr("Room Temperature")
                font.bold: true
                color: settingsPage.textColor
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 16

                Dial {
                    id: tempDial
                    from: 16
                    to: 30
                    value: 22
                    Layout.preferredWidth: 110
                    Layout.preferredHeight: 110
                    onMoved: {
                        console.log("Temperature set to:", Math.round(value), "C")
                    }
                }

                Label {
                    text: Math.round(tempDial.value) + " °C"
                    font.pixelSize: 18
                    font.bold: true
                    color: settingsPage.textColor
                }
            }
        }

        CheckBox {
            text: qsTr("Enable Notifications")
            checked: true
            onCheckedChanged: {
                console.log("Notifications:", checked ? "Enabled" : "Disabled")
            }
        }

        Item { Layout.fillHeight: true }
        Button {
            id: saveButton
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 180
            text: qsTr("Save Settings")

            background: Rectangle {
                radius: 8
                color: settingsPage.primaryColor
            }
            contentItem: Label {
                text: saveButton.text
                color: "white"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: {
                console.log("Settings saved (simulated)")
            }
        }
    }
}
