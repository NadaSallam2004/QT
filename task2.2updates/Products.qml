import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window
    width: 480
    height: 760
    visible: true
    title: "Products"

    readonly property color bgColor: "#F7FAFC"
    readonly property color skinCareColor: "#60A5FA"
    readonly property color hairCareColor: "#34D399"
    readonly property color makeupColor: "#F472B6"

    property int selectedIndex: -1
    property int cartCount: 0
    property string activeCategory: "All"

    ListModel {
        id: productModel
        ListElement {
            name: "Facial Cream"; category: "Skin Care"; price: "500 EGP"
            details: "Type: Cream\nColor: White\nFor all skin types, 50ml jar."
            image: "https://5.imimg.com/data5/JJ/JP/XF/SELLER-27424251/ayurvedic-skin-cream-500x500.jpg"
        }
        ListElement {
            name: "Body Lotion"; category: "Skin Care"; price: "750 EGP"
            details: "Type: Lotion\nColor: -\nDeep moisturizing, 250ml bottle."
            image: "https://static.vecteezy.com/system/resources/previews/055/739/089/non_2x/3-blank-shampoo-bottles-of-beige-color-on-a-white-background-with-folded-towels-and-fresh-flowers-photo.jpg"
        }
        ListElement {
            name: "Sunscreen SPF50"; category: "Skin Care"; price: "370 EGP"
            details: "Type: Sunscreen\nColor: -\nBroad spectrum protection, 100ml."
            image: "https://media.istockphoto.com/id/1488237263/photo/sunscreen-and-glasses-in-a-wicker-straw-bag-on-a-sandy-beach.jpg?s=612x612&w=0&k=20&c=R2zXqYiRu8P19wMxBP73rF2NOHJehscQsjGXWybNBlE="
        }
        ListElement {
            name: "Shampoo"; category: "Hair Care"; price: "190 EGP"
            details: "Type: Shampoo\nColor: -\nFor all hair types, 400ml."
            image: "https://thumbs.dreamstime.com/b/two-nameless-plastic-bottles-white-pink-under-cosmetic-product-background-186276679.jpg"
        }
        ListElement {
            name: "Conditioner"; category: "Hair Care"; price: "295 EGP"
            details: "https://sc04.alicdn.com/kf/H00f616828bca4d85993aeed611f3d801u.jpeg"
            image: "https://picsum.photos/seed/conditioner/300/300"
        }
        ListElement {
            name: "Hair Oil"; category: "Hair Care"; price: "110 EGP"
            details: "Type: CoconutOil\nColor: Amber\nNourishing blend, 100ml."
            image: "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTzKFmNLVTCS2DwOAq68aSoaUfiMzLWTblR_llvopitg-pYP-jNdckNKKxI&s=10"
        }
        ListElement {
            name: "Lipstick"; category: "Makeup"; price: "420 EGP"
            details: "Type: Lipstick\nColor: Red\nMatte finish, long lasting."
            image: "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQ2i6EwfRz73Bxuk1ou0CpoQBTGLfo1aOHPd2b9qUisFw&s=10"
        }
        ListElement {
            name: "Foundation"; category: "Makeup"; price: "780 EGP"
            details: "Type: Foundation\nColor: Beige\nFull coverage, 30ml."
            image: "https://i.pinimg.com/474x/d8/6a/58/d86a5858b99d6b42344b93bfe0b8e479.jpg"
        }
        ListElement {
            name: "Mascara"; category: "Makeup"; price: "500 EGP"
            details: "Type: Mascara\nColor: Black\nVolumizing formula."
            image: "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQaEXhP80Rn3hqRg3n6C-kI2Ra4IWjTYoLkr0P5Ut7XR2SCjP64fjlwPZM-&s=10"
        }
    }

    function categoryColor(category) {
        if (category === "Skin Care") return skinCareColor
        if (category === "Hair Care") return hairCareColor
        if (category === "Makeup") return makeupColor
        return "#9CA3AF"
    }

    ListModel { id: filteredModel }

    function refreshFilter() {
        filteredModel.clear()
        for (var i = 0; i < productModel.count; i++) {
            var item = productModel.get(i)
            if (window.activeCategory === "All" || item.category === window.activeCategory) {
                filteredModel.append({
                    origIndex: i,
                    name: item.name,
                    category: item.category,
                    price: item.price,
                    details: item.details,
                    image: item.image
                })
            }
        }
    }

    Component.onCompleted: refreshFilter()
    onActiveCategoryChanged: refreshFilter()

    background: Rectangle { color: window.bgColor }

    header: ToolBar {
        background: Rectangle { color: "white" }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 16
            anchors.rightMargin: 16

            Text {
                text: "Products"
                font.pixelSize: 20
                font.bold: true
                color: "#1F2937"
                Layout.fillWidth: true
            }

            Rectangle {
                radius: 14
                color: "#F472B6"
                Layout.preferredWidth: cartRow.implicitWidth + 20
                Layout.preferredHeight: 28

                RowLayout {
                    id: cartRow
                    anchors.centerIn: parent
                    spacing: 6

                    Text { text: "🛒"; font.pixelSize: 14 }
                    Text {
                        text: window.cartCount
                        color: "white"
                        font.bold: true
                        font.pixelSize: 13
                    }
                }
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 14

        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            Repeater {
                model: ["All", "Skin Care", "Hair Care", "Makeup"]
                delegate: Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 34
                    radius: 17
                    color: window.activeCategory === modelData
                           ? (modelData === "All" ? "#4B5563" : window.categoryColor(modelData))
                           : "#E5E7EB"

                    Text {
                        anchors.centerIn: parent
                        text: modelData
                        font.pixelSize: 12
                        font.bold: true
                        color: window.activeCategory === modelData ? "white" : "#374151"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            window.activeCategory = modelData
                            window.selectedIndex = -1
                        }
                    }

                    Behavior on color { ColorAnimation { duration: 120 } }
                }
            }
        }

        GridView {
            id: grid
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            cellWidth: width / 3
            cellHeight: cellWidth * 1.35
            model: filteredModel

            delegate: Item {
                width: grid.cellWidth
                height: grid.cellHeight

                ProductCard {
                    anchors.fill: parent
                    anchors.margins: 6
                    productName: name
                    category: model.category
                    price: model.price
                    image: model.image
                    categoryColor: window.categoryColor(model.category)
                    selected: window.selectedIndex === model.origIndex
                    onClicked: window.selectedIndex = model.origIndex
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 150
            radius: 12
            color: "white"
            border.width: 1
            border.color: "#E5E7EB"
            visible: window.selectedIndex >= 0

            RowLayout {
                anchors.fill: parent
                anchors.margins: 14
                spacing: 14

                Rectangle {
                    Layout.preferredWidth: 90
                    Layout.preferredHeight: 90
                    radius: 10
                    color: "#F3F4F6"
                    clip: true

                    Image {
                        anchors.fill: parent
                        source: window.selectedIndex >= 0 ? productModel.get(window.selectedIndex).image : ""
                        fillMode: Image.PreserveAspectCrop
                        asynchronous: true
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 4

                    Text {
                        text: window.selectedIndex >= 0 ? productModel.get(window.selectedIndex).name : ""
                        font.bold: true
                        font.pixelSize: 15
                        color: "#1F2937"
                    }

                    Text {
                        text: window.selectedIndex >= 0 ? productModel.get(window.selectedIndex).details : ""
                        wrapMode: Text.WordWrap
                        font.pixelSize: 12
                        color: "#4B5563"
                        Layout.fillWidth: true
                    }

                    Item { Layout.fillHeight: true }

                    RowLayout {
                        Layout.fillWidth: true

                        Text {
                            text: window.selectedIndex >= 0 ? productModel.get(window.selectedIndex).price : ""
                            font.bold: true
                            font.pixelSize: 14
                            color: "#059669"
                            Layout.fillWidth: true
                        }

                        Button {
                            text: "Add to Cart"
                            onClicked: {
                                window.cartCount++
                                var item = productModel.get(window.selectedIndex)
                                popupText.text = item.name + " added to cart"
                                notificationPopup.open()
                            }
                        }
                    }
                }
            }
        }
    }

    Popup {
        id: notificationPopup
        x: (window.width - width) / 2
        y: 16
        width: 280
        height: 50
        modal: false
        focus: false

        background: Rectangle {
            color: "#374151"
            radius: 10
        }

        Text {
            id: popupText
            anchors.centerIn: parent
            color: "white"
            font.pixelSize: 13
            font.bold: true
        }

        Timer {
            id: popupTimer
            interval: 1500
            repeat: false
            onTriggered: notificationPopup.close()
        }

        onOpened: popupTimer.start()
    }
}
