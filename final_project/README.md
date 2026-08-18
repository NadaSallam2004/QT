# Audio Player Live

A Qt Quick (QML) media player extending the original `audio_player_live`
skeleton with four source types behind **one** playback engine: local files,
internet radio streams, USB media, and Bluetooth audio output.

## Why one engine, four sources

The single biggest source of bugs in players like this is treating each
source as its own playback path. Here there is exactly one:
`MediaPlayerBackend` (a thin, well-tested wrapper around `QMediaPlayer` +
`QAudioOutput`). Local files, USB files, and radio streams all just become a
`QUrl` handed to that one class — `playLocalFile()`, `playUsbFile()`, and
`playRadioStream()` are the same call with a different label attached, so
"local playback works but radio doesn't" style bugs have nowhere to hide.

```
main.cpp
├── MediaPlayerBackend      one QMediaPlayer/QAudioOutput, all sources
├── AudioOutputModel        lists output sinks incl. connected Bluetooth
├── MediaLibraryModel       one mixed playlist (local + USB + radio)
├── UsbVolumeWatcher        polls for removable volumes, scans for media
└── BluetoothManager        OPTIONAL: scan/pair new BT devices (see below)

Main.qml
├── qml/SourceSelector.qml  add local files / radio station / scan USB
├── qml/PlayerControls.qml  transport, seek, volume, output-device picker
├── qml/PlaylistPanel.qml   the mixed playlist
└── qml/BluetoothPanel.qml  scan/pair UI (hidden if BT discovery not built)
```

## Building

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Requires Qt 6.8+ with the **Quick** and **Multimedia** modules (same
requirement as the original project).

## USB media

No special USB protocol is implemented, and deliberately so: once the OS
mounts a USB stick as a filesystem, `UsbVolumeWatcher` just polls
`QStorageInfo::mountedVolumes()` every 1.5s to notice it, then walks the
volume with `QDirIterator` to find playable files. This works unmodified on
Windows/macOS/Linux because it never talks to the device directly — it lets
the OS's own driver stack do that, which is also why it's stable.

## Bluetooth — read this part

Bluetooth audio has two genuinely separate halves, and this project keeps
them separate on purpose:

1. **Playing audio through an already-connected Bluetooth speaker/
   headset/car-kit.** This needs *no extra module*. Once a device is
   paired and connected at the OS level, it shows up as a normal entry in
   `AudioOutputModel` (built on `Qt6::Multimedia`, already linked) — the
   user just picks it from the "Output" dropdown like any other speaker.
   This part works out of the box.

2. **Scanning for and pairing a *new* Bluetooth device from inside the
   app.** This needs the separate **Qt Bluetooth** module
   (`Qt6::Bluetooth`), which is a different Qt component than Multimedia
   and is **off by default** in this project. Turn it on with:

   ```bash
   cmake -S . -B build -DENABLE_BLUETOOTH_DISCOVERY=ON
   ```

   That flag pulls in `BluetoothManager.{h,cpp}` and links
   `Qt6::Bluetooth`. If that module isn't installed in your Qt kit, install
   it via the Qt Maintenance Tool / your package manager before building
   with this flag.

### Platform permission steps for in-app Bluetooth scanning

| Platform | What's needed |
|---|---|
| **macOS** | Add `NSBluetoothAlwaysUsageDescription` (and `NSBluetoothPeripheralUsageDescription` on older SDKs) to the app's `Info.plist` with a user-facing reason string, or macOS silently denies scanning. |
| **iOS** | Same `Info.plist` key as macOS; the app must also be signed with a provisioning profile that doesn't restrict Bluetooth. |
| **Android** | Add `BLUETOOTH_SCAN` and `BLUETOOTH_CONNECT` to `AndroidManifest.xml` (API 31+), or `BLUETOOTH`/`BLUETOOTH_ADMIN`/`ACCESS_FINE_LOCATION` on older targets, and request the runtime permission before calling `startScan()`. |
| **Linux** | Needs BlueZ running (`bluetoothd`) and the user in the `bluetooth`/`lp` group on most distros; no extra manifest step. |
| **Windows** | Needs the Bluetooth radio enabled in Windows settings; no manifest step, but Windows will show its own consent prompt on first pairing. |

Without these, `BluetoothManager::startScan()` will simply fail — 
`adapterAvailable` will read `false` or `errorOccurred` will fire with a
descriptive message; it will not crash.

If you don't need in-app scanning (e.g. all your target devices get paired
once via the OS's own Bluetooth settings), leave
`ENABLE_BLUETOOTH_DISCOVERY` off — playback through Bluetooth still works
via path (1) above, with a smaller dependency footprint.

## Stability notes

- **Radio reconnect**: `MediaPlayerBackend` auto-retries a dropped radio
  stream with exponential backoff (1s → 30s cap, 6 attempts) instead of
  just dying on the first network hiccup. The UI shows a small
  "Reconnecting…" indicator rather than an error state during that window.
- **Seeking a live stream** is a no-op instead of an error — some backends
  throw on `setPosition()` for a live source.
- **Output device removal mid-playback** (e.g. Bluetooth speaker powering
  off): `AudioOutputModel` listens for `QMediaDevices::audioOutputsChanged`
  and falls back to the system default output automatically so playback
  doesn't just go silent.
- **USB scanning** is user-triggered (a button), not automatic, so a large
  stick never makes the UI look like it's hung.
