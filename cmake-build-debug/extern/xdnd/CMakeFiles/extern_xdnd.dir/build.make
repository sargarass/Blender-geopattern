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
include extern/xdnd/CMakeFiles/extern_xdnd.dir/depend.make

# Include the progress variables for this target.
include extern/xdnd/CMakeFiles/extern_xdnd.dir/progress.make

# Include the compile flags for this target's objects.
include extern/xdnd/CMakeFiles/extern_xdnd.dir/flags.make

extern/xdnd/CMakeFiles/extern_xdnd.dir/xdnd.c.o: extern/xdnd/CMakeFiles/extern_xdnd.dir/flags.make
extern/xdnd/CMakeFiles/extern_xdnd.dir/xdnd.c.o: ../extern/xdnd/xdnd.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sargarass/Cycles/geopattern/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object extern/xdnd/CMakeFiles/extern_xdnd.dir/xdnd.c.o"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/extern/xdnd && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/extern_xdnd.dir/xdnd.c.o   -c /home/sargarass/Cycles/geopattern/extern/xdnd/xdnd.c

extern/xdnd/CMakeFiles/extern_xdnd.dir/xdnd.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/extern_xdnd.dir/xdnd.c.i"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/extern/xdnd && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sargarass/Cycles/geopattern/extern/xdnd/xdnd.c > CMakeFiles/extern_xdnd.dir/xdnd.c.i

extern/xdnd/CMakeFiles/extern_xdnd.dir/xdnd.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/extern_xdnd.dir/xdnd.c.s"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/extern/xdnd && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sargarass/Cycles/geopattern/extern/xdnd/xdnd.c -o CMakeFiles/extern_xdnd.dir/xdnd.c.s

# Object files for target extern_xdnd
extern_xdnd_OBJECTS = \
"CMakeFiles/extern_xdnd.dir/xdnd.c.o"

# External object files for target extern_xdnd
extern_xdnd_EXTERNAL_OBJECTS =

lib/libextern_xdnd.a: extern/xdnd/CMakeFiles/extern_xdnd.dir/xdnd.c.o
lib/libextern_xdnd.a: extern/xdnd/CMakeFiles/extern_xdnd.dir/build.make
lib/libextern_xdnd.a: extern/xdnd/CMakeFiles/extern_xdnd.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sargarass/Cycles/geopattern/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library ../../lib/libextern_xdnd.a"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/extern/xdnd && $(CMAKE_COMMAND) -P CMakeFiles/extern_xdnd.dir/cmake_clean_target.cmake
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/extern/xdnd && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/extern_xdnd.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
extern/xdnd/CMakeFiles/extern_xdnd.dir/build: lib/libextern_xdnd.a

.PHONY : extern/xdnd/CMakeFiles/extern_xdnd.dir/build

extern/xdnd/CMakeFiles/extern_xdnd.dir/clean:
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/extern/xdnd && $(CMAKE_COMMAND) -P CMakeFiles/extern_xdnd.dir/cmake_clean.cmake
.PHONY : extern/xdnd/CMakeFiles/extern_xdnd.dir/clean

extern/xdnd/CMakeFiles/extern_xdnd.dir/depend:
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sargarass/Cycles/geopattern /home/sargarass/Cycles/geopattern/extern/xdnd /home/sargarass/Cycles/geopattern/cmake-build-debug /home/sargarass/Cycles/geopattern/cmake-build-debug/extern/xdnd /home/sargarass/Cycles/geopattern/cmake-build-debug/extern/xdnd/CMakeFiles/extern_xdnd.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : extern/xdnd/CMakeFiles/extern_xdnd.dir/depend

