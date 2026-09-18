#
# qcefium library
#
set(CMAKE_AUTOMOC ON)

file(GLOB QCEFIUM_PUBLIC_HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE QCEFIUM_SRC_FILES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

file(GLOB_RECURSE CEFCORE_SRC_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/cefcore/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/cefcore/*.cpp"
)
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR}
  PREFIX Source
  FILES ${CEFCORE_SRC_FILES}
)

if(USE_SANDBOX AND(OS_WINDOWS OR OS_MACOS))
  add_definitions(-DCEF_USE_SANDBOX)
endif()

if(OS_WINDOWS)
  file(GLOB_RECURSE QCEFIUM_WIN_SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/win/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/win/*.cpp"
  )
  source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/win"
    PREFIX Source
    FILES ${QCEFIUM_WIN_SRC_FILES}
  )

  add_library(qcefium ${QCEFIUM_LIB_TYPE}
    ${QCEFIUM_PUBLIC_HEADERS}
    ${QCEFIUM_SRC_FILES}
    ${CEFCORE_SRC_FILES}
    ${QCEFIUM_WIN_SRC_FILES}
  )

  target_compile_definitions(qcefium PRIVATE
    UNICODE
    _UNICODE
    QCEFIUM_LIB
  )

  if(BUILD_STATIC)
    target_link_options(qcefium
      PUBLIC
      "/DELAYLOAD:libcef.dll"
    )

    target_link_libraries(qcefium
      PUBLIC
      d3d11
      d3dcompiler
      dcomp
      delayimp
    )
  else()
    target_link_options(qcefium
      PRIVATE
      "/DELAYLOAD:libcef.dll"
    )

    target_link_libraries(qcefium
      PRIVATE
      d3d11
      d3dcompiler
      dcomp
      delayimp
    )
  endif()

endif()

target_include_directories(qcefium PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_include_directories(qcefium PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CEF_ROOT}"
  "${CEF_ROOT}/include"
)

target_compile_definitions(qcefium PRIVATE
  QCEFIUM_LIBRARY
  ${CEF_COMPILER_DEFINES}
)

target_link_libraries(qcefium PRIVATE
  libcef_lib
  libcef_dll_wrapper
  ${CEF_STANDARD_LIBS}
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Widgets
)

if(OS_WINDOWS)
  # place the CEF binaries and resources next to the library
  COPY_FILES(qcefium "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "$<TARGET_FILE_DIR:qcefium>")
  COPY_FILES(qcefium "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "$<TARGET_FILE_DIR:qcefium>")
endif()

install(TARGETS qcefium
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/" DESTINATION include
  FILES_MATCHING PATTERN "*.h"
)