# 🛍️ Products Catalog (Qt Quick)

A small Qt Quick app that shows a 3-item product category grid — **Skin Care, Hair Care, Makeup** — each with its own image, loaded asynchronously from the network. Tapping a category reveals a details card underneath with its color, type, and price, and a **Disappear** button to hide it again.

<p align="center">
  <img src="screenshots/products_grid.jpeg" width="260" alt="Products grid, no category selected"/>
  <img src="screenshots/skincare_details.jpeg" width="260" alt="Skin Care selected, details card shown"/>
</p>
<p align="center">
  <img src="screenshots/haircare_details.jpeg" width="260" alt="Hair Care selected, details card shown"/>
  <img src="screenshots/makeup_details.jpeg" width="260" alt="Makeup selected, details card shown"/>
</p>

---

## How it behaves

1. **On launch**, a red-bordered grid shows three category cards (`GridLayout` inside a `Repeater`), each with an `Image` loaded asynchronously (`asynchronous: true`, `cache: true`) from a remote URL.
2. While each image downloads, a `BusyIndicator` spins over that specific card — bound directly to `catImage.status === Image.Loading`.
3. **Tapping a card** (`MouseArea`) sets `selectedCategory` to that category and flips `showDetails` to `true`. The selected card also gets a light-blue background (`#ebf8ff`) so the current pick is obvious at a glance.
4. A blue-bordered details panel underneath becomes `visible` and shows the category name plus its `Color / Type / Price` (pulled from a simple lookup table, `productsInfo`).
5. The **Disappear** button just sets `showDetails = false`, hiding the panel without losing the selection.

Like the splash screen example, there's no extra state to keep in sync — `showDetails`, `selectedCategory`, and the busy indicators are the only pieces of state, and everything else is a direct binding off them.

---

## Stack

- **C++17**
- **Qt 6.8+** — `Quick`, `Network` (product images are fetched over HTTP)
- **QML** — `QtQuick`, `QtQuick.Controls`, `QtQuick.Layouts`
- **CMake** build system

---

## Project layout

```
task3/
├── CMakeLists.txt      # Build config; links Qt6::Quick + Qt6::Network
├── main.cpp             # Minimal entry point, loads Main.qml
├── Main.qml              # Splash-style entry window
├── Products.qml          # Product grid + details card (this feature)
├── importedcontent/      # Qt Design Studio / Figma import placeholder (unused here)
└── screenshots/          # README preview images
```

---

## Running it

Requires Qt 6.8+ with the `Quick` and `Network` modules, and an active internet connection (product images are fetched from remote URLs).

```bash
mkdir build && cd build
cmake ..
cmake --build .
./apptask3
```

---

## Ideas for later

- Cache product images locally so the grid works fully offline
- Add more categories/products instead of the fixed three
- Animate the details card in/out instead of toggling `visible` instantly
