#
# QCefDemo
#
set(CMAKE_AUTOMOC ON)

add_executable(QCefDemo WIN32 main.cpp)

target_link_libraries(QCefDemo PRIVATE
  qcefium
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Widgets
)
