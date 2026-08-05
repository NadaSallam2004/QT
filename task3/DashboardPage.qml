import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: dashboardPage

    signal openSettingsRequested()

    readonly property color bgColor: "#FFECC6"     // Pale Sand
    readonly property color cardColor: "#EDCDAF"   // Sand
    readonly property color headerColor: "#654321" // Mocha
    readonly property color textColor: "#3D2D13"   // Umber

    background: Rectangle { color: dashboardPage.bgColor }

    header: ToolBar {
        background: Rectangle { color: dashboardPage.headerColor }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 12
            anchors.rightMargin: 12

            Image {
                source: "images/home.svg"
                Layout.preferredWidth: 28
                Layout.preferredHeight: 28
                fillMode: Image.PreserveAspectFit
            }

            Label {
                text: "Smart Home Dashboard"
                color: "white"
                font.bold: true
                font.pixelSize: 17
                Layout.fillWidth: true
            }

            Button {
                text: "Settings"
                onClicked: dashboardPage.openSettingsRequested()
            }
        }
    }

    property var devicesModel: [
        { name: "Living Room Light", image: "images/light.svg",  usage: 60 },
        { name: "Bedroom Light",     image: "images/light.svg",  usage: 30 },
        { name: "Air Conditioner",   image: "images/ac.svg",     usage: 85 },
        { name: "Fan",                image: "images/fan.svg",    usage: 45 },
        { name: "Garage Door",       image: "images/garage.svg", usage: 10 }
    ]

    ScrollView {
        anchors.fill: parent
        clip: true
        contentWidth: availableWidth

        ColumnLayout {
            width: dashboardPage.width
            spacing: 12

            Item { Layout.preferredHeight: 8 }

            Repeater {
                model: dashboardPage.devicesModel

                delegate: Rectangle {
                    Layout.fillWidth: true
                    Layout.leftMargin: 12
                    Layout.rightMargin: 12
                    Layout.preferredHeight: 92
                    radius: 12
                    color: dashboardPage.cardColor

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 14

                        Image {
                            source: modelData.image
                            Layout.preferredWidth: 50
                            Layout.preferredHeight: 50
                            fillMode: Image.PreserveAspectFit
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 6

                            Label {
                                text: modelData.name
                                font.pixelSize: 15
                                font.bold: true
                                color: dashboardPage.textColor
                            }

                            ProgressBar {
                                Layout.fillWidth: true
                                from: 0
                                to: 100
                                value: modelData.usage
                            }
                        }

                        Switch {
                            checked: true
                            onCheckedChanged: {
                                console.log(modelData.name + " turned " + (checked ? "ON" : "OFF"))
                            }
                        }
                    }
                }
            }

            Item { Layout.preferredHeight: 8 }
        }
    }
}
