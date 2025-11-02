cmake_minimum_required(VERSION 3.22)
project(CalcPlusPlus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Sql)

# Define source files (only .cpp files for add_executable when AUTOMOC is ON)
set(APP_SRCS
    src/main.cpp
    src/ui/MainWindow.cpp
    src/ui/HistoryPanel.cpp
    src/ui/HistoryItemWidget.cpp
    src/core/CalculatorCore.cpp
    src/core/DatabaseManager.cpp
    src/utils/ErrorHandler.cpp
    src/utils/CustomAlert.cpp
)

# Define header files (for IDEs to parse, AUTOMOC will find Q_OBJECT macros automatically)
set(APP_HEADERS
    src/ui/MainWindow.h
    src/ui/HistoryPanel.h
    src/ui/HistoryItemWidget.h
    src/core/CalculatorCore.h
    src/core/DatabaseManager.h
    src/utils/ErrorHandler.h
    src/utils/CustomAlert.h
)

add_executable(${PROJECT_NAME} ${APP_SRCS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui Qt6::Sql
)

# --- Installation Configuration ---

# 1. Install the compiled binary
install(TARGETS ${PROJECT_NAME} DESTINATION /usr/local/bin)

# 2. Create and install the .desktop launcher file
set(CALCPLUSPLUS_DESKTOP_CONTENT "
[Desktop Entry]
Version=1.0
Type=Application
Name=CalcPlusPlus
Comment=Modern calculator app built with Qt6
Exec=CalcPlusPlus
Icon=calcplusplus
Terminal=false
Categories=Utility;Calculator;
StartupNotify=true
")

# Write the .desktop file to the build directory
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/calcplusplus.desktop" "${CALCPLUSPLUS_DESKTOP_CONTENT}")

# Install the .desktop file
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/calcplusplus.desktop"
    DESTINATION share/applications
)

# 3. Install the application icon
# IMPORTANT: Ensure a valid 256x256 PNG icon named 'calcplusplus.png' exists in resources/icons/
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/resources/icons") # Ensure directory exists
install(FILES "${CMAKE_SOURCE_DIR}/resources/icons/calcplusplus.png"
    DESTINATION share/icons/hicolor/256x256/apps
)

# --- CPack Configuration for Debian Package ---

include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "calcplusplus")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_CONTACT "Diego Alejandro Botina <code.with.botina@gmail.com>")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Diego Alejandro Botina")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_ARCHITECTURE "amd64")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Modern calculator built with Qt6 and C++")

# Point CPack to the custom debian/control file
set(CPACK_DEBIAN_PACKAGE_CONTROL_FILE "${CMAKE_SOURCE_DIR}/debian/control")

# Include postinst and prerm scripts
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_SOURCE_DIR}/debian/postinst;${CMAKE_SOURCE_DIR}/debian/prerm")

# Define the base name for the package file (e.g., calcplusplus_1.0.0_amd64)
set(CPACK_PACKAGE_FILE_NAME "calcplusplus_${CPACK_PACKAGE_VERSION}_${CPACK_DEBIAN_ARCHITECTURE}")

# Explicitly set the full path and name for the DEB package output
set(CPACK_OUTPUT_FILE_NAMES "${CMAKE_BINARY_DIR}/${CPACK_PACKAGE_FILE_NAME}.deb")

# Include CPack last
include(CPack)
