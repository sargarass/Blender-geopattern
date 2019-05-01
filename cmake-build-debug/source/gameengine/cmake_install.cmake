# Install script for directory: /home/sargarass/Cycles/geopattern/source/gameengine

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/sargarass/Cycles/geopattern/cmake-build-debug/bin")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/gameengine/BlenderRoutines/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/gameengine/Converter/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/gameengine/Expressions/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/gameengine/GameLogic/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/gameengine/Ketsji/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/gameengine/Ketsji/KXNetwork/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/gameengine/Network/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/gameengine/Network/LoopBackNetwork/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/gameengine/Physics/Dummy/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/gameengine/Rasterizer/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/gameengine/Rasterizer/RAS_OpenGLRasterizer/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/gameengine/SceneGraph/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/gameengine/Physics/Bullet/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/gameengine/VideoTexture/cmake_install.cmake")

endif()

