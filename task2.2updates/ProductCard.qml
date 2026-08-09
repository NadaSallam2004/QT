import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
Rectangle {
    id: card

    property string productName: ""
    property string category: ""
    property string price: ""
    property string image: ""
    property color categoryColor: "#93C5FD"
    property bool selected: false

    signal clicked()

    radius: 14
    color: "white"
    border.width: selected ? 2 : 1
    border.color: selected ? categoryColor : "#E5E7EB"

    Rectangle {
        anchors.fill: parent
        anchors.topMargin: 3
        radius: parent.radius
        color: "#10000000"
        z: -1
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 6

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: card.height * 0.55
            radius: 10
            color: "#F3F4F6"
            clip: true

            Image {
                id: productImage
                anchors.fill: parent
                source: card.image
                fillMode: Image.PreserveAspectCrop
                asynchronous: true
                cache: true
            }

            BusyIndicator {
                anchors.centerIn: parent
                running: productImage.status === Image.Loading
                visible: running
                width: 24
                height: 24
            }

            Rectangle {
                visible: productImage.status === Image.Error
                anchors.fill: parent
                color: "#F3F4F6"
                Text {
                    anchors.centerIn: parent
                    text: "📦"
                    font.pixelSize: 28
                }
            }

            Rectangle {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 6
                radius: 6
                color: card.categoryColor
                width: catLabel.implicitWidth + 10
                height: catLabel.implicitHeight + 6

                Text {
                    id: catLabel
                    anchors.centerIn: parent
                    text: card.category
                    color: "white"
                    font.pixelSize: 10
                    font.bold: true
                }
            }
        }

        Text {
            Layout.fillWidth: true
            text: card.productName
            font.pixelSize: 13
            font.bold: true
            color: "#1F2937"
            elide: Text.ElideRight
            maximumLineCount: 1
        }

        Text {
            Layout.fillWidth: true
            text: card.price
            font.pixelSize: 12
            font.bold: true
            color: "#059669"
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: card.clicked()
    }

    Behavior on border.color { ColorAnimation { duration: 120 } }
}
