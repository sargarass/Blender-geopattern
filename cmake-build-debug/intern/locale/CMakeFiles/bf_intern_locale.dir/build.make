# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/sargarass/Programs/clion-2019.1/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/sargarass/Programs/clion-2019.1/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sargarass/Cycles/geopattern

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sargarass/Cycles/geopattern/cmake-build-debug

# Include any dependencies generated for this target.
include intern/locale/CMakeFiles/bf_intern_locale.dir/depend.make

# Include the progress variables for this target.
include intern/locale/CMakeFiles/bf_intern_locale.dir/progress.make

# Include the compile flags for this target's objects.
include intern/locale/CMakeFiles/bf_intern_locale.dir/flags.make

intern/locale/CMakeFiles/bf_intern_locale.dir/boost_locale_wrapper.cpp.o: intern/locale/CMakeFiles/bf_intern_locale.dir/flags.make
intern/locale/CMakeFiles/bf_intern_locale.dir/boost_locale_wrapper.cpp.o: ../intern/locale/boost_locale_wrapper.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sargarass/Cycles/geopattern/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object intern/locale/CMakeFiles/bf_intern_locale.dir/boost_locale_wrapper.cpp.o"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/intern/locale && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bf_intern_locale.dir/boost_locale_wrapper.cpp.o -c /home/sargarass/Cycles/geopattern/intern/locale/boost_locale_wrapper.cpp

intern/locale/CMakeFiles/bf_intern_locale.dir/boost_locale_wrapper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bf_intern_locale.dir/boost_locale_wrapper.cpp.i"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/intern/locale && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sargarass/Cycles/geopattern/intern/locale/boost_locale_wrapper.cpp > CMakeFiles/bf_intern_locale.dir/boost_locale_wrapper.cpp.i

intern/locale/CMakeFiles/bf_intern_locale.dir/boost_locale_wrapper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bf_intern_locale.dir/boost_locale_wrapper.cpp.s"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/intern/locale && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sargarass/Cycles/geopattern/intern/locale/boost_locale_wrapper.cpp -o CMakeFiles/bf_intern_locale.dir/boost_locale_wrapper.cpp.s

# Object files for target bf_intern_locale
bf_intern_locale_OBJECTS = \
"CMakeFiles/bf_intern_locale.dir/boost_locale_wrapper.cpp.o"

# External object files for target bf_intern_locale
bf_intern_locale_EXTERNAL_OBJECTS =

lib/libbf_intern_locale.a: intern/locale/CMakeFiles/bf_intern_locale.dir/boost_locale_wrapper.cpp.o
lib/libbf_intern_locale.a: intern/locale/CMakeFiles/bf_intern_locale.dir/build.make
lib/libbf_intern_locale.a: intern/locale/CMakeFiles/bf_intern_locale.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sargarass/Cycles/geopattern/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../../lib/libbf_intern_locale.a"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/intern/locale && $(CMAKE_COMMAND) -P CMakeFiles/bf_intern_locale.dir/cmake_clean_target.cmake
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/intern/locale && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bf_intern_locale.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
intern/locale/CMakeFiles/bf_intern_locale.dir/build: lib/libbf_intern_locale.a

.PHONY : intern/locale/CMakeFiles/bf_intern_locale.dir/build

intern/locale/CMakeFiles/bf_intern_locale.dir/clean:
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/intern/locale && $(CMAKE_COMMAND) -P CMakeFiles/bf_intern_locale.dir/cmake_clean.cmake
.PHONY : intern/locale/CMakeFiles/bf_intern_locale.dir/clean

intern/locale/CMakeFiles/bf_intern_locale.dir/depend:
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sargarass/Cycles/geopattern /home/sargarass/Cycles/geopattern/intern/locale /home/sargarass/Cycles/geopattern/cmake-build-debug /home/sargarass/Cycles/geopattern/cmake-build-debug/intern/locale /home/sargarass/Cycles/geopattern/cmake-build-debug/intern/locale/CMakeFiles/bf_intern_locale.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : intern/locale/CMakeFiles/bf_intern_locale.dir/depend

