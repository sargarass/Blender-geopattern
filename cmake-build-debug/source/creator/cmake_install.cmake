# Install script for directory: /home/sargarass/Cycles/geopattern/source/creator

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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(REMOVE_RECURSE 2.79)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79" TYPE DIRECTORY FILES "/home/sargarass/Cycles/geopattern/release/scripts" REGEX "/\\.git$" EXCLUDE REGEX "/\\.gitignore$" EXCLUDE REGEX "/\\.arcconfig$" EXCLUDE REGEX "/\\_\\_pycache\\_\\_$" EXCLUDE REGEX "/addons\\_contrib\\/[^/]*$" EXCLUDE REGEX "/\\_freestyle\\/[^/]*$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles" TYPE DIRECTORY FILES "/home/sargarass/Cycles/geopattern/release/datafiles/fonts")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/release/datafiles/locale/languages")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/ar/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/ar.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/ca/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/ca.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/cs/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/cs.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/de/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/de.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/eo/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/eo.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/es/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/es.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/es_ES/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/es_ES.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/eu/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/eu.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/fa/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/fa.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/fr/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/fr.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/he/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/he.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/hi/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/hi.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/hr/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/hr.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/hu/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/hu.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/id/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/id.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/it/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/it.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/ja/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/ja.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/ko/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/ko.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/ky/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/ky.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/nl/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/nl.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/pl/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/pl.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/pt/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/pt.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/pt_BR/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/pt_BR.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/ru/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/ru.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/sr/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/sr.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/sr@latin/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/sr@latin.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/sv/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/sv.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/tr/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/tr.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/uk/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/uk.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/vi/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/vi.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/zh_CN/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/zh_CN.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/datafiles/locale/zh_TW/LC_MESSAGES" TYPE FILE RENAME "blender.mo" FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/source/creator/zh_TW.mo")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./blender" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./blender")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./blender"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE EXECUTABLE FILES "/home/sargarass/Cycles/geopattern/cmake-build-debug/bin/blender")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./blender" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./blender")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./blender")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE FILES
    "/home/sargarass/Cycles/geopattern/release/freedesktop/blender.desktop"
    "/home/sargarass/Cycles/geopattern/release/freedesktop/icons/scalable/apps/blender.svg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE PROGRAM FILES "/home/sargarass/Cycles/geopattern/release/bin/blender-thumbnailer.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/python/bin" TYPE PROGRAM FILES "/usr/bin/python3.5m")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/python/lib64" TYPE DIRECTORY FILES "/usr/lib64/python3.5" REGEX "/\\_\\_pycache\\_\\_$" EXCLUDE REGEX "/config\\-3\\.5m\\/[^/]*\\.a$" EXCLUDE REGEX "/lib2to3$" EXCLUDE REGEX "/site\\-packages\\/[^/]*$" EXCLUDE REGEX "/tkinter$" EXCLUDE REGEX "/lib\\-dynload\\/\\_tkinter\\.[^/]*$" EXCLUDE REGEX "/idlelib$" EXCLUDE REGEX "/test$" EXCLUDE REGEX "/turtledemo$" EXCLUDE REGEX "/turtle\\.py$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/python/include/python3.5m" TYPE FILE FILES "/usr/include/python3.5m/pyconfig.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/python/lib64/python3.5/site-packages" TYPE DIRECTORY FILES "/usr/lib64/python3.5/site-packages/numpy" REGEX "/\\.svn$" EXCLUDE REGEX "/\\_\\_pycache\\_\\_$" EXCLUDE REGEX "/[^/]*\\.pyc$" EXCLUDE REGEX "/[^/]*\\.pyo$" EXCLUDE REGEX "/oldnumeric$" EXCLUDE REGEX "/doc$" EXCLUDE REGEX "/tests$" EXCLUDE REGEX "/f2py$" EXCLUDE REGEX "/include$" EXCLUDE REGEX "/[^/]*\\.h$" EXCLUDE REGEX "/[^/]*\\.a$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/python/lib64/python3.5/site-packages" TYPE DIRECTORY FILES "/usr/lib64/python3.5/site-packages/requests" REGEX "/\\.svn$" EXCLUDE REGEX "/\\_\\_pycache\\_\\_$" EXCLUDE REGEX "/[^/]*\\.pyc$" EXCLUDE REGEX "/[^/]*\\.pyo$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/python/lib64/python3.5/site-packages" TYPE DIRECTORY FILES "/usr/lib64/python3.5/site-packages/certifi" REGEX "/\\.svn$" EXCLUDE REGEX "/\\_\\_pycache\\_\\_$" EXCLUDE REGEX "/[^/]*\\.pyc$" EXCLUDE REGEX "/[^/]*\\.pyo$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/python/lib64/python3.5/site-packages" TYPE DIRECTORY FILES "/usr/lib64/python3.5/site-packages/chardet" REGEX "/\\.svn$" EXCLUDE REGEX "/\\_\\_pycache\\_\\_$" EXCLUDE REGEX "/[^/]*\\.pyc$" EXCLUDE REGEX "/[^/]*\\.pyo$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/python/lib64/python3.5/site-packages" TYPE DIRECTORY FILES "/usr/lib64/python3.5/site-packages/idna" REGEX "/\\.svn$" EXCLUDE REGEX "/\\_\\_pycache\\_\\_$" EXCLUDE REGEX "/[^/]*\\.pyc$" EXCLUDE REGEX "/[^/]*\\.pyo$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/python/lib64/python3.5/site-packages" TYPE DIRECTORY FILES "/usr/lib64/python3.5/site-packages/urllib3" REGEX "/\\.svn$" EXCLUDE REGEX "/\\_\\_pycache\\_\\_$" EXCLUDE REGEX "/[^/]*\\.pyc$" EXCLUDE REGEX "/[^/]*\\.pyo$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/python/lib64/python3.5/site-packages" TYPE FILE FILES "/usr/lib64/python3.5/site-packages/six.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  
		file(READ "/home/sargarass/Cycles/geopattern/release/text/readme.html" DATA_SRC)
		string(REGEX REPLACE "BLENDER_VERSION" "2.79" DATA_DST "${DATA_SRC}")
		file(WRITE "/home/sargarass/Cycles/geopattern/cmake-build-debug/release/text/readme.html" "${DATA_DST}")
		unset(DATA_SRC)
		unset(DATA_DST)
		
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE FILES
    "/home/sargarass/Cycles/geopattern/release/text/GPL-license.txt"
    "/home/sargarass/Cycles/geopattern/release/text/GPL3-license.txt"
    "/home/sargarass/Cycles/geopattern/release/text/Python-license.txt"
    "/home/sargarass/Cycles/geopattern/release/text/copyright.txt"
    "/home/sargarass/Cycles/geopattern/release/text/jemalloc-license.txt"
    "/home/sargarass/Cycles/geopattern/release/text/ocio-license.txt"
    "/home/sargarass/Cycles/geopattern/release/datafiles/LICENSE-bfont.ttf.txt"
    "/home/sargarass/Cycles/geopattern/release/datafiles/LICENSE-droidsans.ttf.txt"
    "/home/sargarass/Cycles/geopattern/release/datafiles/LICENSE-bmonofont-i18n.ttf.txt"
    "/home/sargarass/Cycles/geopattern/cmake-build-debug/release/text/readme.html"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/blender/addon/__init__.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/blender/addon/engine.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/blender/addon/osl.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/blender/addon/presets.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/blender/addon/properties.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/blender/addon/ui.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/blender/addon/version_update.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/license" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/doc/license/Apache_2.0.txt")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/license" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/doc/license/ILM.txt")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/license" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/doc/license/NVidia.txt")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/license" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/doc/license/OSL.txt")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/license" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/doc/license/Sobol.txt")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/license" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/doc/license/readme.txt")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_state_buffer_size.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_split.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_data_init.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_path_init.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_queue_enqueue.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_scene_intersect.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_lamp_emission.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_do_volume.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_indirect_background.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_shader_setup.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_shader_sort.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_shader_eval.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_holdout_emission_blurring_pathtermination_ao.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_subsurface_scatter.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_direct_lighting.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_shadow_blocked_ao.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_shadow_blocked_dl.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_enqueue_inactive.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_next_iteration_setup.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_indirect_subsurface.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_buffer_update.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/kernel_split_function.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/opencl" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/opencl/filter.cl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/cuda" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/cuda/kernel.cu")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/cuda" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/cuda/kernel_split.cu")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/cuda" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/cuda/filter.cu")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_accumulate.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_bake.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_camera.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_compat_cpu.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_compat_cuda.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_compat_opencl.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_debug.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_differential.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_emission.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_film.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_globals.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_image_opencl.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_jitter.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_light.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_math.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_montecarlo.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_passes.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_path.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_path_branched.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_path_common.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_path_state.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_path_surface.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_path_subsurface.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_path_volume.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_projection.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_queues.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_random.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_shader.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_shadow.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_subsurface.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_textures.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_types.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_volume.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernel_work_stealing.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/kernels/cuda" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/kernels/cuda/kernel_config.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/bvh" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/bvh/bvh.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/bvh" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/bvh/bvh_nodes.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/bvh" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/bvh/bvh_shadow_all.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/bvh" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/bvh/bvh_shadow_all_cuda.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/bvh" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/bvh/bvh_subsurface.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/bvh" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/bvh/bvh_traversal.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/bvh" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/bvh/bvh_traversal_cuda.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/bvh" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/bvh/bvh_traversal_cuda_types.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/bvh" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/bvh/bvh_types.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/bvh" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/bvh/bvh_volume.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/bvh" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/bvh/bvh_volume_all.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/bvh" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/bvh/qbvh_nodes.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/bvh" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/bvh/qbvh_shadow_all.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/bvh" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/bvh/qbvh_subsurface.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/bvh" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/bvh/qbvh_traversal.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/bvh" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/bvh/qbvh_volume.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/bvh" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/bvh/qbvh_volume_all.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/alloc.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/bsdf.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/bsdf_ashikhmin_velvet.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/bsdf_diffuse.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/bsdf_diffuse_ramp.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/bsdf_microfacet.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/bsdf_microfacet_multi.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/bsdf_microfacet_multi_impl.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/bsdf_oren_nayar.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/bsdf_phong_ramp.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/bsdf_reflection.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/bsdf_refraction.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/bsdf_toon.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/bsdf_transparent.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/bsdf_util.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/bsdf_ashikhmin_shirley.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/bsdf_hair.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/bssrdf.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/emissive.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/volume.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/bsdf_principled_diffuse.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/closure" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/closure/bsdf_principled_sheen.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/filter" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/filter/filter.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/filter" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/filter/filter_defines.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/filter" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/filter/filter_features.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/filter" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/filter/filter_features_sse.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/filter" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/filter/filter_kernel.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/filter" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/filter/filter_nlm_cpu.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/filter" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/filter/filter_nlm_gpu.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/filter" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/filter/filter_prefilter.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/filter" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/filter/filter_reconstruction.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/filter" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/filter/filter_transform.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/filter" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/filter/filter_transform_gpu.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/filter" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/filter/filter_transform_sse.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_attribute.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_blackbody.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_bump.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_camera.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_closure.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_convert.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_checker.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_color_util.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_brick.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_displace.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_fresnel.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_wireframe.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_wavelength.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_gamma.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_brightness.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_geometry.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_gradient.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_hsv.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_image.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_invert.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_light_path.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_magic.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_mapping.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_math.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_math_util.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_mix.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_musgrave.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_noise.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_noisetex.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_normal.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_ramp.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_ramp_util.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_sepcomb_hsv.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_sepcomb_vector.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_sky.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_tex_coord.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_texture.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_types.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_value.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_vector_transform.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_voronoi.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_voxel.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/svm" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/svm/svm_wave.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/geom" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/geom/geom.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/geom" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/geom/geom_attribute.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/geom" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/geom/geom_curve.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/geom" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/geom/geom_motion_curve.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/geom" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/geom/geom_motion_triangle.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/geom" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/geom/geom_motion_triangle_intersect.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/geom" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/geom/geom_motion_triangle_shader.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/geom" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/geom/geom_object.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/geom" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/geom/geom_patch.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/geom" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/geom/geom_primitive.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/geom" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/geom/geom_subd_triangle.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/geom" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/geom/geom_triangle.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/geom" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/geom/geom_triangle_intersect.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/geom" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/geom/geom_volume.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_atomic.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_color.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_half.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_hash.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_mat.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_math.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_math_fast.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_math_intersect.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_math_float2.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_math_float3.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_math_float4.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_math_int2.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_math_int3.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_math_int4.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_math_matrix.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_static_assert.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_transform.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_texture.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_float2.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_float2_impl.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_float3.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_float3_impl.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_float4.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_float4_impl.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_int2.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_int2_impl.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_int3.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_int3_impl.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_int4.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_int4_impl.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_uchar2.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_uchar2_impl.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_uchar3.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_uchar3_impl.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_uchar4.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_uchar4_impl.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_uint2.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_uint2_impl.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_uint3.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_uint3_impl.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_uint4.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_uint4_impl.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_vector3.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/util" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/../util/util_types_vector3_impl.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_branched.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_buffer_update.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_data_init.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_direct_lighting.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_do_volume.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_enqueue_inactive.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_holdout_emission_blurring_pathtermination_ao.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_indirect_background.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_indirect_subsurface.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_lamp_emission.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_next_iteration_setup.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_path_init.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_queue_enqueue.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_scene_intersect.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_shader_setup.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_shader_sort.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_shader_eval.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_shadow_blocked_ao.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_shadow_blocked_dl.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_split_common.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_split_data.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_split_data_types.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/2.79/scripts/addons/cycles/source/kernel/split" TYPE FILE FILES "/home/sargarass/Cycles/geopattern/intern/cycles/kernel/split/kernel_subsurface_scatter.h")
endif()

