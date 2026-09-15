#
# LiveDemo
#
set(CMAKE_AUTOMOC ON)

add_executable(LiveDemo WIN32 main.cpp)

target_link_libraries(LiveDemo PRIVATE
  livecef
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Widgets
)
