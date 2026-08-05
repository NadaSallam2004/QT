# 👋 Hello World App (Qt Quick)

A minimal Qt Quick app that greets a user and shows their info in a card. No backend, no network — just a handful of QML `property string` values feeding two simple `Text` blocks.

<p align="center">
  <img src="screenshots/hello_world_app.png" width="380" alt="Hello world greeting + info card"/>
</p>

---

## How it behaves

1. Five plain `property string` values (`userName`, `userRole`, `userUniversity`, `userCity`, `userAddress`) hold the user's data — no model, no JSON, just properties on the `ApplicationWindow`.
2. A green-bordered **greeting card** builds a multi-line sentence by string-concatenating `userName`, `userRole`, and `userCity` straight into a single `Text` element.
3. A second **info card** underneath lists every property as its own `→ label: value` row, again just plain string concatenation per line.
4. Both cards sit in a `Column` with `anchors.centerIn: parent`, so the whole thing stays centered regardless of window size.

There's no interactivity by design — it's a static display of bound properties, useful as a starting point before wiring the values up to real input or a backend.

---

## Stack

- **C++17**
- **Qt 6.8+** — `Quick`
- **QML** — `QtQuick`, `QtQuick.Controls`
- **CMake** build system

---

## Project layout

```
task1_qt/
├── CMakeLists.txt      # Build config; links Qt6::Quick
├── main.cpp             # Minimal entry point, loads Main.qml
├── Main.qml              # Greeting card + info card, all in one file
├── importedcontent/      # Qt Design Studio / Figma import placeholder (unused here)
└── screenshots/          # README preview images
```

---

## Running it

Requires Qt 6.8+ with the `Quick` module.

```bash
mkdir build && cd build
cmake ..
cmake --build .
./apptask1_qt
```

---

## Ideas for later

- Turn the hard-coded properties into editable `TextField`s
- Load user data from a JSON file or REST API instead of literals
- Add a second user and let a `ComboBox` switch between them
