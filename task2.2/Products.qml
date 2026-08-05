import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    width: 480
    height: 700
    visible: true
    title: "Products"

    property string selectedCategory: ""
    property bool showDetails: false

    property var productsInfo: ({
        "Skin Care": "Color: White\nType: Cream\nPrice: 150 EGP",
        "Hair Care": "Color: -\nType: Shampoo\nPrice: 90 EGP",
        "Makeup":    "Color: Red\nType: Lipstick\nPrice: 120 EGP"
    })

    property var productsImages: ({
        "Skin Care": "https://img.freepik.com/premium-photo/3d-render-individual-skincare-product-package-white-blank-mockup-generative-ai_1002555-2515.jpg",
        "Hair Care": "https://as2.ftcdn.net/v2/jpg/07/25/02/03/1000_F_725020314_zGtQmiZhaD0MDGduey4bORSOcOzWYbyY.jpg",
        "Makeup":    "https://img.freepik.com/premium-photo/makeup-products-studio-shot-cosmetics-studio-background-floating-eye-shadows-lipstick-closeup-shot_723234-5466.jpg"
    })

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        Text {
            text: "Products"
            font.pixelSize: 22
            font.bold: true
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 250
            border.color: "red"
            border.width: 2
            radius: 8

            GridLayout {
                anchors.fill: parent
                anchors.margins: 10
                columns: 3
                rowSpacing: 10
                columnSpacing: 10

                Repeater {
                    model: ["Skin Care", "Hair Care", "Makeup"]
                    delegate: Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        border.color: "red"
                        border.width: 1
                        radius: 6
                        color: selectedCategory === modelData ? "#ebf8ff" : "white"

                        Image {
                            id: catImage
                            anchors.fill: parent
                            anchors.margins: 8
                            source: productsImages[modelData]
                            fillMode: Image.PreserveAspectFit
                            asynchronous: true
                            cache: true
                        }

                        BusyIndicator {
                            anchors.centerIn: parent
                            running: catImage.status === Image.Loading
                            visible: running
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                selectedCategory = modelData
                                showDetails = true
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 180
            border.color: "blue"
            border.width: 2
            radius: 8
            visible: showDetails

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 10

                RowLayout {
                    Layout.fillWidth: true

                    Text {
                        text: selectedCategory
                        font.bold: true
                        font.pixelSize: 16
                        Layout.fillWidth: true
                    }

                    Button {
                        text: "Disappear"
                        onClicked: showDetails = false
                    }
                }

                Text {
                    text: productsInfo[selectedCategory] || ""
                    wrapMode: Text.WordWrap
                    font.pixelSize: 14
                }
            }
        }
    }
}
