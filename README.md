# 📊 Sorting Visualizer

A real-time sorting algorithm visualizer built with **C++** and **SFML 3.0** — watch 5 classic sorting algorithms animate step by step!

![Sorting Visualizer](https://raw.githubusercontent.com/snehal-thombare08/Sorting-Visualizer/main/Screenshot%202026-06-17%20141219.png)

## ✨ Features

- **5 Sorting Algorithms** — Bubble, Insertion, Merge, Quick, Selection
- **Animated step-by-step** — every comparison and swap is visualized
- **Color coding** — Red = comparing, Yellow = swapping, Rainbow = sorted!
- **Speed control** — slow down or speed up with arrow keys
- **Fast mode** — instant sort with F key
- **Step counter** — see exactly how many operations each algorithm takes
- **120 bars** for smooth, detailed visualization

## 🚀 Download & Run

1. Download **SortingVisualizer-v1.0-Windows.zip**
2. Extract all files
3. Run **SortingVisualizer.exe**

## 🎮 Controls

| Key | Action |
|-----|--------|
| `1` | Bubble Sort |
| `2` | Insertion Sort |
| `3` | Merge Sort |
| `4` | Quick Sort |
| `5` | Selection Sort |
| `Space` | Shuffle & restart |
| `F` | Toggle fast mode |
| `P` | Pause / Resume |
| `↑` / `↓` | Increase / Decrease speed |
| `Esc` | Quit |

## 🛠️ Build from Source

**Requirements:** C++17, SFML 3.0, CMake, MinGW

```bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=<vcpkg>/scripts/buildsystems/vcpkg.cmake -G "MinGW Makefiles"
mingw32-make
```

## 📈 Algorithm Comparison

| Algorithm | Best | Average | Worst | Steps (N=120) |
|-----------|------|---------|-------|---------------|
| Bubble Sort | O(n) | O(n²) | O(n²) | ~10,000+ |
| Insertion Sort | O(n) | O(n²) | O(n²) | ~7,000+ |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) | ~1,500+ |
| Quick Sort | O(n log n) | O(n log n) | O(n²) | ~900+ |
| Selection Sort | O(n²) | O(n²) | O(n²) | ~7,000+ |

## 🧰 Tech Stack

- **Language:** C++17
- **Graphics:** SFML 3.0
- **Build:** CMake + MinGW
