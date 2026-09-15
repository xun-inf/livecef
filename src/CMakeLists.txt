#
# livecef library
#
set(CMAKE_AUTOMOC ON)

file(GLOB LIVECEF_PUBLIC_HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE LIVECEF_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(livecef ${LIVECEF_LIB_TYPE}
  ${LIVECEF_PUBLIC_HEADERS}
  ${LIVECEF_SOURCES}
)
add_library(livecef::livecef ALIAS livecef)

target_include_directories(livecef PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_include_directories(livecef PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CEF_ROOT}"
  "${CEF_ROOT}/include"
)

target_compile_definitions(livecef PRIVATE
  LIVECEF_LIBRARY
  ${CEF_COMPILER_DEFINES}
)

target_link_libraries(livecef PRIVATE
  libcef_lib
  libcef_dll_wrapper
  ${CEF_STANDARD_LIBS}
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Widgets
)

if(OS_WINDOWS)
  # place the CEF binaries and resources next to the library
  COPY_FILES(livecef "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "$<TARGET_FILE_DIR:livecef>")
  COPY_FILES(livecef "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "$<TARGET_FILE_DIR:livecef>")
endif()

install(TARGETS livecef
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/" DESTINATION include
  FILES_MATCHING PATTERN "*.h"
)