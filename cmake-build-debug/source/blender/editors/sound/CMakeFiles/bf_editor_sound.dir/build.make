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
include source/blender/editors/sound/CMakeFiles/bf_editor_sound.dir/depend.make

# Include the progress variables for this target.
include source/blender/editors/sound/CMakeFiles/bf_editor_sound.dir/progress.make

# Include the compile flags for this target's objects.
include source/blender/editors/sound/CMakeFiles/bf_editor_sound.dir/flags.make

source/blender/editors/sound/CMakeFiles/bf_editor_sound.dir/sound_ops.c.o: source/blender/editors/sound/CMakeFiles/bf_editor_sound.dir/flags.make
source/blender/editors/sound/CMakeFiles/bf_editor_sound.dir/sound_ops.c.o: ../source/blender/editors/sound/sound_ops.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sargarass/Cycles/geopattern/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object source/blender/editors/sound/CMakeFiles/bf_editor_sound.dir/sound_ops.c.o"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/sound && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bf_editor_sound.dir/sound_ops.c.o   -c /home/sargarass/Cycles/geopattern/source/blender/editors/sound/sound_ops.c

source/blender/editors/sound/CMakeFiles/bf_editor_sound.dir/sound_ops.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bf_editor_sound.dir/sound_ops.c.i"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/sound && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sargarass/Cycles/geopattern/source/blender/editors/sound/sound_ops.c > CMakeFiles/bf_editor_sound.dir/sound_ops.c.i

source/blender/editors/sound/CMakeFiles/bf_editor_sound.dir/sound_ops.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bf_editor_sound.dir/sound_ops.c.s"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/sound && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sargarass/Cycles/geopattern/source/blender/editors/sound/sound_ops.c -o CMakeFiles/bf_editor_sound.dir/sound_ops.c.s

# Object files for target bf_editor_sound
bf_editor_sound_OBJECTS = \
"CMakeFiles/bf_editor_sound.dir/sound_ops.c.o"

# External object files for target bf_editor_sound
bf_editor_sound_EXTERNAL_OBJECTS =

lib/libbf_editor_sound.a: source/blender/editors/sound/CMakeFiles/bf_editor_sound.dir/sound_ops.c.o
lib/libbf_editor_sound.a: source/blender/editors/sound/CMakeFiles/bf_editor_sound.dir/build.make
lib/libbf_editor_sound.a: source/blender/editors/sound/CMakeFiles/bf_editor_sound.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sargarass/Cycles/geopattern/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library ../../../../lib/libbf_editor_sound.a"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/sound && $(CMAKE_COMMAND) -P CMakeFiles/bf_editor_sound.dir/cmake_clean_target.cmake
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/sound && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bf_editor_sound.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
source/blender/editors/sound/CMakeFiles/bf_editor_sound.dir/build: lib/libbf_editor_sound.a

.PHONY : source/blender/editors/sound/CMakeFiles/bf_editor_sound.dir/build

source/blender/editors/sound/CMakeFiles/bf_editor_sound.dir/clean:
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/sound && $(CMAKE_COMMAND) -P CMakeFiles/bf_editor_sound.dir/cmake_clean.cmake
.PHONY : source/blender/editors/sound/CMakeFiles/bf_editor_sound.dir/clean

source/blender/editors/sound/CMakeFiles/bf_editor_sound.dir/depend:
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sargarass/Cycles/geopattern /home/sargarass/Cycles/geopattern/source/blender/editors/sound /home/sargarass/Cycles/geopattern/cmake-build-debug /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/sound /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/sound/CMakeFiles/bf_editor_sound.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : source/blender/editors/sound/CMakeFiles/bf_editor_sound.dir/depend

