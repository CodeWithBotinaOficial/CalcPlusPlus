# CalcPlusPlus - A Modern Qt6 C++ Scientific Calculator

## Table of Contents
- [Overview](#overview)
- [Features](#features)
  - [Core Functionality](#core-functionality)
  - [Expression Display](#expression-display)
  - [Interactive History](#interactive-history)
  - [Error Handling](#error-handling)
  - [Design & User Interface](#design--user-interface)
- [Installation Guide](#installation-guide)
  - [Option 1: Install from .deb package (Recommended)](#option-1-install-from-deb-package-recommended)
  - [Option 2: Build Manually](#option-2-build-manually)
- [Technical Details](#technical-details)
- [License](#license)
- [Author](#author)

---

## Overview
CalcPlusPlus is a professional, modern, and intuitive scientific calculator application developed using C++17 and the Qt6 framework. Designed for Linux (Ubuntu 22.04+), it offers a clean user interface, responsive layout, and persistent calculation history, making it a reliable tool for both basic and advanced mathematical operations.

- **Version:** v1.0.0
- **Platform:** Linux (Ubuntu 22.04+)
- **Built with:** C++17, Qt6 (Widgets, Core, GUI, SQL), CMake, SQLite
- **Packaging:** `.deb` installer generated via CPack

---

## Features

### Core Functionality
-   **Comprehensive Operations:** Perform addition, subtraction, multiplication, division, percentages, exponentiation (x^y), and square roots.
-   **Precision Handling:** Supports decimal values and negative numbers with double-precision floating-point arithmetic.
-   **Robustness:** Includes integrated error handling to gracefully manage invalid expressions and mathematical exceptions like division by zero.

### Expression Display
CalcPlusPlus features an intuitive dual-line display for clarity:
-   **Top Line:** Shows the full mathematical expression as it's being entered or processed (e.g., `75 × 3 + 2`).
-   **Bottom Line:** Displays the current number being input or the partial/final result (e.g., `227`).
-   **Post-Calculation View:** After pressing the equals button (`=`), the complete expression (e.g., `75 × 3 + 2 =`) remains visible in a smaller, semi-transparent style above the final result, providing a clear record of the performed calculation.

### Interactive History
A dedicated history panel, integrated seamlessly as a `QDockWidget`, provides a comprehensive record of all past calculations:
-   **Persistent Storage:** All operations (expression and result) are automatically saved to a local SQLite database (`calc_history.db`), ensuring history is retained across application sessions.
-   **Scrollable List:** Displays entries in a scrollable list, with each item showing the operation and its result.
-   **Recall Functionality:** Clicking any history entry loads that specific expression and its result back into the main calculator display, allowing users to easily reuse or continue from previous calculations.
-   **Clear History:** A convenient "Clear History" button is available within the panel to delete all stored entries.

### Error Handling
-   **User-Friendly Alerts:** Replaces generic system message boxes with custom-styled alert dialogs that match the application's dark theme.
-   **Clear Feedback:** Provides user-friendly error messages for invalid expressions, mathematical exceptions (e.g., `sqrt(-1)`), and operational issues.

### Design & User Interface
-   **Modern Dark Theme:** Features a sleek dark theme with consistent typography, spacing, and color palette for an aesthetically pleasing experience.
-   **Responsive Layout:** The window layout is designed to adapt gracefully to different display sizes, maintaining usability and visual appeal.
-   **Intuitive Button Arrangement:** Buttons are arranged in a standard, easy-to-use calculator layout for efficient input.

---

## Installation Guide

### Option 1: Install from .deb package (Recommended)
The easiest way to install CalcPlusPlus on Ubuntu 22.04+ is by using the provided Debian package.

1.  **Download the `.deb` file:**
    Obtain the latest `calcplusplus_1.0.0_amd64.deb` file from the [Releases page](https://github.com/CodeWithBotinaOficial/CalcPlusPlus/releases).

2.  **Install the package:**
    Open a terminal in the directory where you downloaded the `.deb` file and run:
    ```bash
    sudo dpkg -i calcplusplus_1.0.0_amd64.deb
    sudo apt install -f # To resolve any missing dependencies
    ```

3.  **Launch CalcPlusPlus:**
    Once installed, you can launch CalcPlusPlus from your system’s applications menu (look under "Utilities" or "Calculator") or by typing the command:
    ```bash
    CalcPlusPlus
    ```

### Option 2: Build Manually
If you prefer to build CalcPlusPlus from source, follow these steps:

1.  **Install Dependencies:**
    Ensure you have the necessary build tools and Qt6 development libraries installed:
    ```bash
    sudo apt update
    sudo apt install qt6-base-dev qt6-base-dev-tools libsqlite3-dev cmake g++ ninja-build
    ```

2.  **Clone the Repository:**
    ```bash
    git clone https://github.com/CodeWithBotinaOficial/CalcPlusPlus.git
    cd CalcPlusPlus
    ```

3.  **Build the Project:**
    ```bash
    cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt6
    cmake --build build --config Release
    ```

4.  **Run the Executable:**
    ```bash
    ./build/CalcPlusPlus
    ```

---

## Technical Details
-   **Project Structure:**
    -   `src/core`: Contains the core mathematical logic and the SQLite database manager.
    -   `src/ui`: Manages the Qt Widgets-based user interface and window components.
    -   `src/utils`: Provides utility classes for error handling and custom alerts.
    -   `resources/`: Stores application assets like icons and desktop entry files.
-   **Build System:** CMake is used for cross-platform build configuration, with Ninja as the build tool.
-   **Database:** SQLite3 is integrated for persistent storage of calculation history, automatically managed on application startup.
-   **Release Automation:** GitHub Actions are configured to automate the build process, generate `.deb` packages, and publish them to GitHub Releases and GitHub Packages upon new tag pushes.

---

## License
This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.

---

## Author
© 2025 Diego Alejandro Botina — *CodeWithBotina*
