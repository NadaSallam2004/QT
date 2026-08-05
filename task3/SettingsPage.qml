import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: settingsPage

    // ---------- Signal مخصص: بيتبعت لما تدوس على زرار الرجوع ----------
    signal backRequested()

    readonly property color bgColor: "#FFECC6"      // Pale Sand
    readonly property color headerColor: "#654321"  // Mocha
    readonly property color primaryColor: "#BB9351" // Maple
    readonly property color textColor: "#3D2D13"    // Umber

    background: Rectangle { color: settingsPage.bgColor }

    header: ToolBar {
        background: Rectangle { color: settingsPage.headerColor }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 8
            anchors.rightMargin: 12

            Button {
                text: "< Back"
                onClicked: settingsPage.backRequested()
            }

            Label {
                text: "Settings"
                color: "white"
                font.bold: true
                font.pixelSize: 17
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }

            // مساحة فاضية عشان العنوان يفضل في النص فعليًا
            Item { Layout.preferredWidth: 60 }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 28

        // ---------- اللغة ----------
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 6

            Label {
                text: "Application Language"
                font.bold: true
                color: settingsPage.textColor
            }

            ComboBox {
                Layout.fillWidth: true
                model: ["English", "Arabic", "French"]
                onActivated: {
                    // ترجمة فعلية غير مطلوبة، بس نحاكيها بـ log
                    console.log("Language changed to:", currentText)
                }
            }
        }

        // ---------- السطوع ----------
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 6

            Label {
                text: "Screen Brightness"
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

        // ---------- درجة الحرارة ----------
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 6

            Label {
                text: "Room Temperature"
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

        // ---------- الإشعارات ----------
        CheckBox {
            text: "Enable Notifications"
            checked: true
            onCheckedChanged: {
                console.log("Notifications:", checked ? "Enabled" : "Disabled")
            }
        }

        Item { Layout.fillHeight: true }

        // ---------- زرار الحفظ ----------
        Button {
            id: saveButton
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 180
            text: "Save Settings"

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
                // مفيش حفظ حقيقي، بنحاكيه بـ log فقط
                console.log("Settings saved (simulated)")
            }
        }
    }
}
