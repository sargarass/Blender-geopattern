# Install script for directory: /home/sargarass/Cycles/geopattern/source/blender/editors

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
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/animation/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/armature/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/curve/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/gpencil/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/interface/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/io/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/mask/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/mesh/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/metaball/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/object/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/render/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/screen/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/sculpt_paint/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/sound/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/space_action/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/space_api/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/space_buttons/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/space_clip/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/space_console/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/space_file/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/space_graph/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/space_image/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/space_info/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/space_logic/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/space_nla/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/space_node/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/space_outliner/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/space_script/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/space_sequencer/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/space_text/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/space_time/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/space_userpref/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/space_view3d/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/transform/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/util/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/uvedit/cmake_install.cmake")
  include("/home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/datafiles/cmake_install.cmake")

endif()

