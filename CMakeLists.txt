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

# --- Packaging Configuration for CPack (.deb) ---

include(InstallRequiredSystemLibraries)

set(CPACK_PROJECT_NAME "${PROJECT_NAME}") # Explicitly set CPack project name
set(CPACK_PACKAGE_NAME "calcplusplus")
set(CPACK_PACKAGE_VENDOR "CodeWithBotina")
set(CPACK_PACKAGE_CONTACT "code.with.botina@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CalcPlusPlus — A modern and elegant Qt6-based calculator for Linux.")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/CodeWithBotinaOficial/CalcPlusPlus")
set(CPACK_PACKAGE_MAINTAINER "Diego Alejandro Botina (CodeWithBotina)")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}_amd64")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6widgets6, libqt6gui6, libqt6core6, libqt6sql6, libsqlite3-0")
set(CPACK_DEBIAN_ARCHITECTURE "amd64")

# --- Installation Rules ---
# Install the compiled binary to /usr/bin
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Create the resources/icons directory if it doesn't exist
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/resources/icons")
# Install the application icon to /usr/share/icons/hicolor/256x256/apps (Corrected path)
install(FILES "${CMAKE_SOURCE_DIR}/resources/icons/calcplusplus.png" DESTINATION share/icons/hicolor/256x256/apps)

# Create the resources/desktop directory if it doesn't exist
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/resources/desktop")
# Install the .desktop launcher file to /usr/share/applications
install(FILES "${CMAKE_SOURCE_DIR}/resources/desktop/calcplusplus.desktop" DESTINATION share/applications)

# Install LICENSE and README.md
install(FILES "${CMAKE_SOURCE_DIR}/LICENSE" DESTINATION share/doc/${CPACK_PACKAGE_NAME})
install(FILES "${CMAKE_SOURCE_DIR}/README.md" DESTINATION share/doc/${CPACK_PACKAGE_NAME})

# --- CPack Configuration for Debian Package ---
# Set the CPack generator to DEB
set(CPACK_GENERATOR "DEB")

# Point CPack to the custom debian/control file
set(CPACK_DEBIAN_PACKAGE_CONTROL_FILE "${CMAKE_SOURCE_DIR}/debian/control")

# Include postinst and prerm scripts
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_SOURCE_DIR}/debian/postinst;${CMAKE_SOURCE_DIR}/debian/prerm")

# Include CPack last
include(CPack)
