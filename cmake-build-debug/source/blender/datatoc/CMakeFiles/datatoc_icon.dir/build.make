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
include source/blender/datatoc/CMakeFiles/datatoc_icon.dir/depend.make

# Include the progress variables for this target.
include source/blender/datatoc/CMakeFiles/datatoc_icon.dir/progress.make

# Include the compile flags for this target's objects.
include source/blender/datatoc/CMakeFiles/datatoc_icon.dir/flags.make

source/blender/datatoc/CMakeFiles/datatoc_icon.dir/datatoc_icon.c.o: source/blender/datatoc/CMakeFiles/datatoc_icon.dir/flags.make
source/blender/datatoc/CMakeFiles/datatoc_icon.dir/datatoc_icon.c.o: ../source/blender/datatoc/datatoc_icon.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sargarass/Cycles/geopattern/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object source/blender/datatoc/CMakeFiles/datatoc_icon.dir/datatoc_icon.c.o"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/datatoc && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/datatoc_icon.dir/datatoc_icon.c.o   -c /home/sargarass/Cycles/geopattern/source/blender/datatoc/datatoc_icon.c

source/blender/datatoc/CMakeFiles/datatoc_icon.dir/datatoc_icon.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/datatoc_icon.dir/datatoc_icon.c.i"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/datatoc && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sargarass/Cycles/geopattern/source/blender/datatoc/datatoc_icon.c > CMakeFiles/datatoc_icon.dir/datatoc_icon.c.i

source/blender/datatoc/CMakeFiles/datatoc_icon.dir/datatoc_icon.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/datatoc_icon.dir/datatoc_icon.c.s"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/datatoc && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sargarass/Cycles/geopattern/source/blender/datatoc/datatoc_icon.c -o CMakeFiles/datatoc_icon.dir/datatoc_icon.c.s

# Object files for target datatoc_icon
datatoc_icon_OBJECTS = \
"CMakeFiles/datatoc_icon.dir/datatoc_icon.c.o"

# External object files for target datatoc_icon
datatoc_icon_EXTERNAL_OBJECTS =

bin/datatoc_icon: source/blender/datatoc/CMakeFiles/datatoc_icon.dir/datatoc_icon.c.o
bin/datatoc_icon: source/blender/datatoc/CMakeFiles/datatoc_icon.dir/build.make
bin/datatoc_icon: /usr/lib/libpng.so
bin/datatoc_icon: /usr/lib/libz.so
bin/datatoc_icon: /usr/lib/libz.so
bin/datatoc_icon: source/blender/datatoc/CMakeFiles/datatoc_icon.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sargarass/Cycles/geopattern/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ../../../bin/datatoc_icon"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/datatoc && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/datatoc_icon.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
source/blender/datatoc/CMakeFiles/datatoc_icon.dir/build: bin/datatoc_icon

.PHONY : source/blender/datatoc/CMakeFiles/datatoc_icon.dir/build

source/blender/datatoc/CMakeFiles/datatoc_icon.dir/clean:
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/datatoc && $(CMAKE_COMMAND) -P CMakeFiles/datatoc_icon.dir/cmake_clean.cmake
.PHONY : source/blender/datatoc/CMakeFiles/datatoc_icon.dir/clean

source/blender/datatoc/CMakeFiles/datatoc_icon.dir/depend:
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sargarass/Cycles/geopattern /home/sargarass/Cycles/geopattern/source/blender/datatoc /home/sargarass/Cycles/geopattern/cmake-build-debug /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/datatoc /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/datatoc/CMakeFiles/datatoc_icon.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : source/blender/datatoc/CMakeFiles/datatoc_icon.dir/depend

