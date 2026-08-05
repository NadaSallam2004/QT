import QtQuick
import QtQuick.Controls

ApplicationWindow {
    width: 480
    height: 600
    visible: true
    title: "Hello World App"

    property string userName: "Ahmed"
    property string userRole: "Student"
    property string userUniversity: "Alexandria University"
    property string userCity: "Alexandria"
    property string userAddress: "12 El-Horreya St."

    Column {
        anchors.centerIn: parent
        spacing: 20

        Rectangle {
            width: 400
            height: 140
            color: "#f0fff4"
            border.color: "green"
            border.width: 2
            radius: 8

            Text {
                anchors.centerIn: parent
                text: "Hello world, I am " + userName + "\n" +
                      "I am " + userRole + "\n" +
                      "Worked at city: " + userCity
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
            }
        }
        Rectangle {
            width: 400
            height: 230
            color: "white"
            border.color: "gray"
            border.width: 1
            radius: 8

            Column {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 10

                Text {
                    text: "Card: Info"
                    font.bold: true
                    font.pixelSize: 18
                }

                Text { text: "→ Name: " + userName }
                Text { text: "→ Role: " + userRole }
                Text { text: "→ University: " + userUniversity }
                Text { text: "→ City: " + userCity }
                Text { text: "→ Address: " + userAddress }
            }
        }
    }
}