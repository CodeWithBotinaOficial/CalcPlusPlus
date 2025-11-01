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

# Install rules for creating a .deb package later
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES
    # Add any other necessary files for deployment here, e.g., icons, config files
    # For now, just the executable
    DESTINATION share/${PROJECT_NAME}
)

# CPack configuration for .deb package
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "CalcPlusPlus")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Diego Botina <code.with.botina@gmail.com>")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "A modern scientific calculator built with Qt6 and C++.")
set(CPACK_DEBIAN_PACKAGE_SECTION "math")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_FILE_NAME "CalcPlusPlus-${CPACK_PACKAGE_VERSION}-Linux.deb")
include(CPack)
