# 🧮 CalcPlusPlus

**CalcPlusPlus** is a modern, professional-grade calculator application built in **C++** with a **Qt6 GUI** and **SQLite** support for persistent history tracking.  
Developed by [Diego Alejandro Botina (CodeWithBotina)](https://github.com/CodeWithBotinaOficial), this project aims to demonstrate clean architecture, strong error handling, and elegant UI design — making it a perfect addition to any software development portfolio.

---

## 🚀 Features

✅ **Basic Operations** — Addition, subtraction, multiplication, division, percentage, power, and square root.  
🧠 **Smart History** — Every equation is saved to an **SQLite** database with navigable history.  
🎨 **Responsive GUI** — Built with **Qt6**, the interface dynamically adjusts to different window sizes.  
⚙️ **Error Handling** — Robust validation for invalid inputs, divide-by-zero, and malformed expressions.  
🧩 **Design Patterns** — Clean and modular architecture following OOP and MVC principles.  
💾 **Precision** — Full floating-point and decimal handling with rounding control.  
🔥 **Cross-Platform Ready** — Built and tested on Ubuntu 22.04; distributed as a `.deb` package.  

---

## 🏗️ Tech Stack

| Component | Technology Used |
|------------|-----------------|
| **Language** | C++17 |
| **GUI Framework** | Qt 6.x |
| **Database** | SQLite3 |
| **Build System** | CMake + GNU Make |
| **Compiler** | g++ 13.1.0 |
| **IDE** | CLion 2025.2.4 |
| **OS Compatibility** | Ubuntu 22.04+ (.deb package release) |

---

## 🧰 Project Structure

```

CalcPlusPlus/
├── CMakeLists.txt
├── include/              # Header files
├── src/                  # Core source code
│   ├── core/             # Business logic (CalculatorCore, ExpressionParser, etc.)
│   ├── ui/               # GUI elements and Qt windows
│   ├── utils/            # Utility functions and classes
│   └── main.cpp          # Entry point
├── assets/               # Icons, stylesheets, etc.
├── database/             # SQLite file and schema (auto-created)
├── .gitignore
├── LICENSE
└── README.md

````

---

## ⚙️ Installation & Setup

### 🧩 Requirements

- **Ubuntu 22.04+**
- **Qt6** (QtCore, QtWidgets, QtSql)
- **CMake ≥ 3.22**
- **g++ ≥ 13.0**
- **SQLite3**

### 🧪 Build Instructions

1. **Clone the repository**
   ```bash
   git clone https://github.com/CodeWithBotinaOficial/CalcPlusPlus.git
   cd CalcPlusPlus
    ```

2. **Configure the build**

   ```bash
   cmake -B build
   ```

3. **Compile the app**

   ```bash
   cmake --build build
   ```

4. **Run it**

   ```bash
   ./build/CalcPlusPlus
   ```

---

## 📦 Creating the .deb Package

To create a `.deb` package for distribution:

```bash
cpack -G DEB
```

This will generate a Debian package in the `build` directory, which you can install with:

```bash
sudo dpkg -i CalcPlusPlus-*.deb
```

---

## 🧠 Design Overview

CalcPlusPlus uses a **modular architecture** that separates logic, data, and presentation:

* **Core Layer (`src/core`)**
  Handles all mathematical computations and error validations.
* **Database Layer (`src/db`)**
  Manages SQLite connections, query execution, and persistent history.
* **UI Layer (`src/ui`)**
  Implements all Qt widgets, signals, and slots for interactive experience.

---

## 💾 Database Schema

The SQLite database stores all user operations for quick access and browsing:

```sql
CREATE TABLE history (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    expression TEXT NOT NULL,
    result TEXT NOT NULL,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
);
```

---

## 👨‍💻 Author

**Diego Alejandro Botina**
Alias: **CodeWithBotina**
🌐 [GitHub Profile](https://github.com/CodeWithBotinaOficial)
💼 Software Developer | C++ | Qt | Systems Enthusiast

---

## 📜 License

This project is licensed under the **MIT License** — see the [LICENSE](./LICENSE) file for details.

---

## ⭐ Contribute & Support

If you like this project, consider giving it a ⭐ on GitHub!
Contributions, pull requests, and feature suggestions are always welcome.

---

🧮 *CalcPlusPlus — a clean, fast, and modern calculator built for developers by developers.*
