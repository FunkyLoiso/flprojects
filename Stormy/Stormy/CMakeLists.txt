project(stormy-ui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)

file(GLOB SRC
  "*.hpp"
  "*.cpp"
  "quartic/*.hpp"
)

add_executable(
  ${PROJECT_NAME}
    ${SRC}
)

qt5_use_modules(
  ${PROJECT_NAME}
    Core Gui Widgets
)
