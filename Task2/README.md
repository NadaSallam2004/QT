# ✨ Splash & Welcome Screen (Qt Quick)

A tiny Qt Quick app built around one idea: **don't just show a spinner — show something that actually knows how it's doing.** A logo loads from the network in the background, a busy indicator spins only while it's genuinely loading, an error message appears only if it genuinely fails, and after a fixed delay the splash steps aside for the real app.

<p align="center">
  <img src="screenshots/splash.png" width="380" alt="Splash screen loading the logo"/>
  <img src="screenshots/welcome.png" width="380" alt="Welcome screen"/>
</p>

---

## How it behaves

1. **On launch**, a full-window splash `Rectangle` sits on top (`z: 10`) with a remotely-hosted logo image in the middle.
2. While the image is downloading, a `BusyIndicator` spins — driven directly off `logoImage.status === Image.Loading`, not a manual flag.
3. If the network request fails, the spinner disappears and an Arabic fallback message (*"تعذّر تحميل الصورة"*) shows instead — again driven off `Image.status === Image.Error`, no extra state to manage.
4. A one-shot `Timer` (3 seconds) hides the splash regardless of whether the image finished, so a slow or dead connection never traps the user on the loading screen.
5. Underneath the whole time, the real app content — a plain "Welcome to the App!" screen — is already loaded and waiting.

The interesting bit isn't the UI, it's that **the splash state machine has zero JavaScript state variables.** Every visual change is a direct binding to `Image.status`, so there's nothing to get out of sync.

---

## Stack

- **C++17**
- **Qt 6.8+** — `Quick`, `Network` (needed since the logo is fetched over HTTP)
- **QML** for the entire UI
- **CMake** build system

---

## Project layout

```
qt_tsk2/
├── CMakeLists.txt      # Build config; links Qt6::Quick + Qt6::Network
├── main.cpp            # Minimal entry point, loads Main.qml
├── Main.qml            # Splash overlay + welcome screen, all in one file
├── importedcontent/    # Empty placeholder Qt Design Studio reserves for Figma imports (unused here)
└── screenshots/        # README preview images
```

---

## Running it

Requires Qt 6.8+ with the `Quick` and `Network` modules, and an active internet connection (the splash logo is fetched from a remote URL — swap `Main.qml`'s `source` for a local asset if you want it to work fully offline).

```bash
mkdir build && cd build
cmake ..
cmake --build .
./appqt_tsk2
```

---

## Ideas for later

- Bundle the logo as a local resource so the splash works with no network at all
- Fade the splash out instead of hiding it instantly
- Make the 3-second delay end early the moment the image finishes loading, instead of always waiting the full duration
