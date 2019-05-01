# Install script for directory: /home/sargarass/Cycles/geopattern/extern

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
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/extern/curve_fit_nd/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/extern/rangetree/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/extern/wcwidth/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/extern/bullet2/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/extern/binreloc/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/extern/recastnavigation/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/extern/lzo/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/extern/lzma/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/extern/clew/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/extern/cuew/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/extern/carve/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/extern/xdnd/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/extern/ceres/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/extern/gflags/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/extern/glog/cmake_install.cmake")

endif()

