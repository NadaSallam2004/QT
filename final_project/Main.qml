import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtCore
import audio_player_live
import "qml/Theme.js" as Theme

import "qml"

ApplicationWindow {
    id: window
    width: 960
    height: 600
    minimumWidth: 720
    minimumHeight: 480
    visible: true
    color: Theme.background
    title: qsTr("Audio Player Live") + (mediaPlayer.title ? " — " + mediaPlayer.title : "")

    Material.theme: Material.Dark
    Material.accent: Theme.accent
    Material.primary: Theme.surface
    Material.background: Theme.background
    Material.foreground: Theme.textPrimary

    Settings {
        id: settings
        category: "player"
        property real lastVolume: 0.8
    }

    Component.onCompleted: mediaPlayer.volume = settings.lastVolume
    Component.onDestruction: settings.lastVolume = mediaPlayer.volume

    StackView {
        id: stack
        anchors.fill: parent

        initialItem: splashComponent

        Component {
            id: splashComponent
            SplashScreen {
                onFinished: stack.replace(homeComponent, StackView.Immediate)
            }
        }

        Component {
            id: homeComponent
            HomeScreen {
                onSourceSelected: function(source) {
                    stack.push(playerComponent, { initialSource: source })
                }
            }
        }

        Component {
            id: playerComponent
            PlayerScreen {
                onBackRequested: stack.pop()
            }
        }
    }
}
