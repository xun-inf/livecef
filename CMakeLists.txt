#
# The main config file for qcefium
#
cmake_minimum_required(VERSION 3.21)
project(qcefium)

set(CMAKE_FOLDER "qcefium")

# options
option(BUILD_DEMO
  "Build the demo"
  OFF
)
option(BUILD_STATIC
  "Build qcefium as static library"
  OFF
)
option(STATIC_CRT
  "Use MultiThreaded linkage for MSVC"
  OFF
)
option(USE_SANDBOX
  "Enable CEF Sandbox"
  OFF
)
option(USE_GPU_OPTIMUS
  "Enable GPU Optimus (Only for Windows and usually not needed)"
  OFF
)
option(USE_WIN_DCOMPOSITION
  "Enabled Windows direct composition for hardware rendering, _WIN32_WINNT >= 0x602 (Windows 8) is required"
  OFF
)

# append cmake config module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Only generate Debug and Release configuration types.
set(CMAKE_CONFIGURATION_TYPES Debug Release)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# Use folders in the resulting project files.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# C standard
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

# C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

add_definitions(-D_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING=1)

# Determine the project architecture.
if(NOT DEFINED PROJECT_ARCH)
  if(OS_WINDOWS AND "${CMAKE_GENERATOR_PLATFORM}" STREQUAL "arm64")
    set(PROJECT_ARCH "arm64")
  elseif(CMAKE_SIZEOF_VOID_P MATCHES 8)
    set(PROJECT_ARCH "x86_64")
  else()
    set(PROJECT_ARCH "x86")
  endif()
endif()

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
  set(OS_MACOS 1)
  set(OS_POSIX 1)
  add_definitions(
    -DOS_MACOS=1
    -DOS_POSIX=1
  )
  add_compile_options(
    "-g"
    "$<$<CONFIG:DEBUG>:-O0>"
    "$<$<CONFIG:RELEASE>:-O3>"
  )

  # Target architecture.
  if(PROJECT_ARCH STREQUAL "x86_64")
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
  elseif(PROJECT_ARCH STREQUAL "arm64")
    set(CMAKE_OSX_ARCHITECTURES "arm64")
  else()
    set(CMAKE_OSX_ARCHITECTURES "i386")
  endif()
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  set(OS_WINDOWS 1)

  # Disable the sandbox on Windows, because the sandbox.lib is MT which is conflict with Qt
  set(USE_SANDBOX OFF CACHE BOOL "Disable sandbox on Windows" FORCE)
  add_definitions(
    -DOS_WINDOWS=1
    -D_WIN32_WINNT=0x602
  )

  if(USE_WIN_DCOMPOSITION)
    add_definitions(
      -DENABLE_WINDOWS_DIRECT_COMPOSITION=1
    )
  endif()

  add_compile_options(
    /W3
    /WX
    /M$<IF:$<BOOL:${STATIC_CRT}>,T,D>$<$<CONFIG:Debug>:d>
  )
  add_link_options(/DEBUG)
endif()

if(BUILD_STATIC)
  set(QCEFIUM_LIB_TYPE STATIC)
  add_definitions(-DQCEFIUM_STATIC=1)
else()
  set(QCEFIUM_LIB_TYPE SHARED)
endif()

# detect whether we are in sub folder
get_directory_property(QCEFIUM_HAS_PARENT_DIRECTORY PARENT_DIRECTORY)

if(NOT QCEFIUM_HAS_PARENT_DIRECTORY)
  message(STATUS "qcefium is not in subdirectory, put all output together")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/$<CONFIG>/bin)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/$<CONFIG>/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/$<CONFIG>/lib)
endif()

# Config the QT package
# ##############################################################
set(QT_SDK_DIR "" CACHE PATH "Qt build toolchain path")
include(QtConfig)
set(Qt_VERSION ${Qt${QT_VERSION_MAJOR}Core_VERSION})

# ##############################################################
# thirdparty cef-builds
add_subdirectory(thirdparty)
include(SetupCef)

if(OS_MACOS)
  # detect minimum deployment target by Qt
  if(${Qt_VERSION} VERSION_GREATER_EQUAL 6.5)
    set(QT_MIN_DEPLOYMENT_TARGET 11.0)
  elseif(${Qt_VERSION} VERSION_GREATER_EQUAL 6.2)
    set(QT_MIN_DEPLOYMENT_TARGET 10.14)
  elseif(${Qt_VERSION} VERSION_GREATER_EQUAL 5.14)
    set(QT_MIN_DEPLOYMENT_TARGET 10.13)
  elseif(${Qt_VERSION} VERSION_GREATER_EQUAL 5.13)
    set(QT_MIN_DEPLOYMENT_TARGET 10.12)
  elseif(${Qt_VERSION} VERSION_GREATER_EQUAL 5.10)
    set(QT_MIN_DEPLOYMENT_TARGET 10.11)
  else()
    set(QT_MIN_DEPLOYMENT_TARGET 10.10)
  endif()

  # detect minimum deployment target by CEF
  # plese refer to: https://bitbucket.org/chromiumembedded/cef/wiki/BranchesAndBuilding.md
  if(${CEF_VERSION_MAJOR} VERSION_GREATER_EQUAL 138)
    set(CEF_MIN_DEPLOYMENT_TARGET 12.0)
  elseif(${CEF_VERSION_MAJOR} VERSION_GREATER_EQUAL 117)
    set(CEF_MIN_DEPLOYMENT_TARGET 10.15)
  elseif(${CEF_VERSION_MAJOR} VERSION_GREATER_EQUAL 104)
    set(CEF_MIN_DEPLOYMENT_TARGET 10.13)
  else()
    set(CEF_MIN_DEPLOYMENT_TARGET 10.11)
  endif()

  # use the greater one as the minimum deployment target
  if(${QT_MIN_DEPLOYMENT_TARGET} VERSION_LESS ${CEF_MIN_DEPLOYMENT_TARGET})
    set(CMAKE_OSX_DEPLOYMENT_TARGET ${CEF_MIN_DEPLOYMENT_TARGET})
  else()
    set(CMAKE_OSX_DEPLOYMENT_TARGET ${QT_MIN_DEPLOYMENT_TARGET})
  endif()
endif()

# Config qcefium target
# ##############################################################
add_subdirectory(src)

# Config the Demo project
# ##############################################################
if(BUILD_DEMO)
  add_subdirectory(example/QCefDemo)
endif()

# ##############################################################
