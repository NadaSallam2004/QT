# 🧩 Qt Learning Journey

This repository is a collection of small Qt Quick / QML projects, built while learning the framework — from a static "Hello World" info card, to a splash-screen state machine, to an interactive product catalog. Each task lives in its own folder with its own README and screenshots; this page is the cover page for the repo as a whole and a quick primer on Qt itself.

---

## What is Qt?

**Qt** (pronounced "cute") is a free and open-source, cross-platform application development framework, originally released in 1995 and now maintained by **The Qt Company**. It's written in C++ and lets developers build the same application once and ship it across desktop, mobile, and embedded platforms with little to no platform-specific code.

Qt is really two things working together:

- **A C++ class library** — windowing, networking, file I/O, databases, multimedia, Bluetooth, and dozens of other modules, all with a consistent, object-oriented API.
- **QML + Qt Quick** — a declarative, JavaScript-like language for describing UI. Instead of imperatively building widgets in C++, you *declare* what the interface looks like and how it reacts to state, which is what every `.qml` file in this repo does.

The two are bridged by Qt's **signals & slots** mechanism and its **property binding system** — the same idea behind why, in these projects, a `BusyIndicator`'s `running` property can just be bound to `Image.status === Image.Loading` instead of managed by hand.

---

## What is Qt used for?

Qt shows up in a surprisingly wide range of software, because "write once, deploy almost anywhere" is genuinely one of its strengths:

- **Desktop applications** — Qt Creator itself, VLC, Wireshark, Telegram Desktop, and large chunks of the KDE desktop environment are built with Qt.
- **Embedded & IoT devices** — kiosks, medical devices, industrial control panels, and smart-home hardware, thanks to Qt's small footprint and support for constrained hardware.
- **Automotive infotainment** — many in-car dashboard and infotainment systems run on Qt for Automotive.
- **Mobile apps** — Qt targets Android and iOS from the same QML/C++ codebase.
- **Scientific & engineering tools** — data visualization dashboards, lab instrument UIs, and simulation front-ends often use Qt for its strong 2D/3D graphics and charting modules.

Its main draw is **one codebase, many targets** — the same `Main.qml` structure used in this repo's "Hello World" task could, with minimal changes, run on a phone, a car dashboard, or a Linux desktop.

---

## Why C++ *and* QML together?

- **C++** handles the heavy lifting — business logic, performance-critical code, networking, hardware access — and is exposed to QML as context properties or registered types.
- **QML** handles the UI layer declaratively, which is faster to iterate on and easier to reskin without touching application logic.

This split is visible even in these small tasks: `main.cpp` is a thin bootstrap that just loads a `.qml` file, and all the actual behavior — properties, layouts, state — lives in QML.

---

## Repo structure

```
.
├── task1_qt/        # Hello World — static user info card, plain property bindings
├── task3/           # Products — category grid + tappable details card
└── qt_tsk2/         # Splash & Welcome screen — async image loading, timers
```

Each folder has its own `README.md` with screenshots and a walkthrough of what that specific task does. This page is just the front door.

---

