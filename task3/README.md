# 🏠 Smart Home Dashboard

A desktop control panel for a smart home, built entirely in **Qt Quick (QML)** with a razor-thin **C++** shell underneath. Log in, flip your devices on and off, watch their usage in real time, and tune the room from one settings screen.

<p align="center">
  <img src="screenshots/login.jpg" width="260" alt="Login screen"/>
  <img src="screenshots/dashboard.jpg" width="420" alt="Dashboard screen"/>
</p>
<p align="center">
  <img src="screenshots/settings.jpg" width="700" alt="Settings screen"/>
</p>

---

## What it does

**Login** — type a username and password, hit *Login*, and a short simulated auth flow (busy spinner + timer) runs before you land on the dashboard. Empty fields are caught instantly with an inline warning instead of a silent failure.

**Dashboard** — every device in the house shows up as its own card: an icon, a live usage bar, and a switch. Living room light, bedroom light, AC, fan, garage door — flip any of them and it logs the state change immediately.

**Settings** — pick an app language, drag the brightness slider, spin the temperature dial to set the room to whatever degree you want, toggle notifications, and save.

Only three screens, but the transitions between them aren't hardcoded into each page. Every page just announces what happened — *login succeeded*, *user opened settings*, *user hit back* — and one central navigation stack decides where to go. That means any of these three screens could be dropped into a completely different app tomorrow and still work exactly the same.

---

## Under the hood

| Layer | Role |
|---|---|
| `main.cpp` | Boots the Qt engine — nothing else. Zero business logic in C++. |
| `Main.qml` | Owns the `StackView` and reacts to signals from each page. |
| `LoginPage.qml` | Auth form, validation, simulated async login. |
| `DashboardPage.qml` | Repeater-driven device list bound to a small in-memory model. |
| `SettingsPage.qml` | ComboBox, Slider, Dial, CheckBox — all standard QtQuick Controls, styled to match the app's palette. |
| `images/` | Hand-picked SVG icons for each device and the user avatar. |

**Stack:** C++17 · Qt 6.5+ (Quick, QuickControls2) · CMake · QML

**Color palette:** a warm sand/mocha theme (`#FFECC6`, `#EDCDAF`, `#654321`, `#BB9351`, `#3D2D13`) instead of the usual blue/gray dashboard look — chosen to make the app feel more like a cozy home interface than an admin panel.

---

## Running it locally

Requires Qt 6.5+ with the `Quick` and `QuickControls2` modules, plus CMake 3.16+.

```bash
mkdir build && cd build
cmake ..
cmake --build .
./SmartHomeDashboard
```

---

## Where this could go next

- Wire the switches to real hardware or an MQTT broker instead of just logging state
- Persist settings and login state between sessions
- Replace the language dropdown with actual translated strings
