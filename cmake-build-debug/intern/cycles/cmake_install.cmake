# Install script for directory: /home/sargarass/Cycles/geopattern/intern/cycles

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
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/intern/cycles/blender/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/intern/cycles/bvh/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/intern/cycles/device/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/intern/cycles/doc/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/intern/cycles/graph/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/intern/cycles/kernel/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/intern/cycles/render/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/intern/cycles/subd/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/intern/cycles/util/cmake_install.cmake")

endif()

