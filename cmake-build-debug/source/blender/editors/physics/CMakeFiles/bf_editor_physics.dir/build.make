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
include source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/depend.make

# Include the progress variables for this target.
include source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/progress.make

# Include the compile flags for this target's objects.
include source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/flags.make

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/dynamicpaint_ops.c.o: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/flags.make
source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/dynamicpaint_ops.c.o: ../source/blender/editors/physics/dynamicpaint_ops.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sargarass/Cycles/geopattern/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/dynamicpaint_ops.c.o"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bf_editor_physics.dir/dynamicpaint_ops.c.o   -c /home/sargarass/Cycles/geopattern/source/blender/editors/physics/dynamicpaint_ops.c

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/dynamicpaint_ops.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bf_editor_physics.dir/dynamicpaint_ops.c.i"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sargarass/Cycles/geopattern/source/blender/editors/physics/dynamicpaint_ops.c > CMakeFiles/bf_editor_physics.dir/dynamicpaint_ops.c.i

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/dynamicpaint_ops.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bf_editor_physics.dir/dynamicpaint_ops.c.s"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sargarass/Cycles/geopattern/source/blender/editors/physics/dynamicpaint_ops.c -o CMakeFiles/bf_editor_physics.dir/dynamicpaint_ops.c.s

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/particle_boids.c.o: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/flags.make
source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/particle_boids.c.o: ../source/blender/editors/physics/particle_boids.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sargarass/Cycles/geopattern/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/particle_boids.c.o"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bf_editor_physics.dir/particle_boids.c.o   -c /home/sargarass/Cycles/geopattern/source/blender/editors/physics/particle_boids.c

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/particle_boids.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bf_editor_physics.dir/particle_boids.c.i"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sargarass/Cycles/geopattern/source/blender/editors/physics/particle_boids.c > CMakeFiles/bf_editor_physics.dir/particle_boids.c.i

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/particle_boids.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bf_editor_physics.dir/particle_boids.c.s"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sargarass/Cycles/geopattern/source/blender/editors/physics/particle_boids.c -o CMakeFiles/bf_editor_physics.dir/particle_boids.c.s

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/particle_edit.c.o: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/flags.make
source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/particle_edit.c.o: ../source/blender/editors/physics/particle_edit.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sargarass/Cycles/geopattern/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/particle_edit.c.o"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bf_editor_physics.dir/particle_edit.c.o   -c /home/sargarass/Cycles/geopattern/source/blender/editors/physics/particle_edit.c

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/particle_edit.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bf_editor_physics.dir/particle_edit.c.i"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sargarass/Cycles/geopattern/source/blender/editors/physics/particle_edit.c > CMakeFiles/bf_editor_physics.dir/particle_edit.c.i

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/particle_edit.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bf_editor_physics.dir/particle_edit.c.s"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sargarass/Cycles/geopattern/source/blender/editors/physics/particle_edit.c -o CMakeFiles/bf_editor_physics.dir/particle_edit.c.s

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/particle_object.c.o: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/flags.make
source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/particle_object.c.o: ../source/blender/editors/physics/particle_object.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sargarass/Cycles/geopattern/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/particle_object.c.o"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bf_editor_physics.dir/particle_object.c.o   -c /home/sargarass/Cycles/geopattern/source/blender/editors/physics/particle_object.c

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/particle_object.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bf_editor_physics.dir/particle_object.c.i"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sargarass/Cycles/geopattern/source/blender/editors/physics/particle_object.c > CMakeFiles/bf_editor_physics.dir/particle_object.c.i

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/particle_object.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bf_editor_physics.dir/particle_object.c.s"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sargarass/Cycles/geopattern/source/blender/editors/physics/particle_object.c -o CMakeFiles/bf_editor_physics.dir/particle_object.c.s

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/physics_fluid.c.o: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/flags.make
source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/physics_fluid.c.o: ../source/blender/editors/physics/physics_fluid.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sargarass/Cycles/geopattern/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/physics_fluid.c.o"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bf_editor_physics.dir/physics_fluid.c.o   -c /home/sargarass/Cycles/geopattern/source/blender/editors/physics/physics_fluid.c

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/physics_fluid.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bf_editor_physics.dir/physics_fluid.c.i"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sargarass/Cycles/geopattern/source/blender/editors/physics/physics_fluid.c > CMakeFiles/bf_editor_physics.dir/physics_fluid.c.i

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/physics_fluid.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bf_editor_physics.dir/physics_fluid.c.s"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sargarass/Cycles/geopattern/source/blender/editors/physics/physics_fluid.c -o CMakeFiles/bf_editor_physics.dir/physics_fluid.c.s

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/physics_ops.c.o: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/flags.make
source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/physics_ops.c.o: ../source/blender/editors/physics/physics_ops.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sargarass/Cycles/geopattern/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/physics_ops.c.o"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bf_editor_physics.dir/physics_ops.c.o   -c /home/sargarass/Cycles/geopattern/source/blender/editors/physics/physics_ops.c

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/physics_ops.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bf_editor_physics.dir/physics_ops.c.i"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sargarass/Cycles/geopattern/source/blender/editors/physics/physics_ops.c > CMakeFiles/bf_editor_physics.dir/physics_ops.c.i

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/physics_ops.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bf_editor_physics.dir/physics_ops.c.s"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sargarass/Cycles/geopattern/source/blender/editors/physics/physics_ops.c -o CMakeFiles/bf_editor_physics.dir/physics_ops.c.s

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/physics_pointcache.c.o: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/flags.make
source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/physics_pointcache.c.o: ../source/blender/editors/physics/physics_pointcache.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sargarass/Cycles/geopattern/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/physics_pointcache.c.o"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bf_editor_physics.dir/physics_pointcache.c.o   -c /home/sargarass/Cycles/geopattern/source/blender/editors/physics/physics_pointcache.c

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/physics_pointcache.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bf_editor_physics.dir/physics_pointcache.c.i"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sargarass/Cycles/geopattern/source/blender/editors/physics/physics_pointcache.c > CMakeFiles/bf_editor_physics.dir/physics_pointcache.c.i

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/physics_pointcache.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bf_editor_physics.dir/physics_pointcache.c.s"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sargarass/Cycles/geopattern/source/blender/editors/physics/physics_pointcache.c -o CMakeFiles/bf_editor_physics.dir/physics_pointcache.c.s

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/rigidbody_constraint.c.o: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/flags.make
source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/rigidbody_constraint.c.o: ../source/blender/editors/physics/rigidbody_constraint.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sargarass/Cycles/geopattern/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/rigidbody_constraint.c.o"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bf_editor_physics.dir/rigidbody_constraint.c.o   -c /home/sargarass/Cycles/geopattern/source/blender/editors/physics/rigidbody_constraint.c

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/rigidbody_constraint.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bf_editor_physics.dir/rigidbody_constraint.c.i"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sargarass/Cycles/geopattern/source/blender/editors/physics/rigidbody_constraint.c > CMakeFiles/bf_editor_physics.dir/rigidbody_constraint.c.i

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/rigidbody_constraint.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bf_editor_physics.dir/rigidbody_constraint.c.s"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sargarass/Cycles/geopattern/source/blender/editors/physics/rigidbody_constraint.c -o CMakeFiles/bf_editor_physics.dir/rigidbody_constraint.c.s

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/rigidbody_object.c.o: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/flags.make
source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/rigidbody_object.c.o: ../source/blender/editors/physics/rigidbody_object.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sargarass/Cycles/geopattern/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/rigidbody_object.c.o"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bf_editor_physics.dir/rigidbody_object.c.o   -c /home/sargarass/Cycles/geopattern/source/blender/editors/physics/rigidbody_object.c

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/rigidbody_object.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bf_editor_physics.dir/rigidbody_object.c.i"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sargarass/Cycles/geopattern/source/blender/editors/physics/rigidbody_object.c > CMakeFiles/bf_editor_physics.dir/rigidbody_object.c.i

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/rigidbody_object.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bf_editor_physics.dir/rigidbody_object.c.s"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sargarass/Cycles/geopattern/source/blender/editors/physics/rigidbody_object.c -o CMakeFiles/bf_editor_physics.dir/rigidbody_object.c.s

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/rigidbody_world.c.o: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/flags.make
source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/rigidbody_world.c.o: ../source/blender/editors/physics/rigidbody_world.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sargarass/Cycles/geopattern/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/rigidbody_world.c.o"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bf_editor_physics.dir/rigidbody_world.c.o   -c /home/sargarass/Cycles/geopattern/source/blender/editors/physics/rigidbody_world.c

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/rigidbody_world.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bf_editor_physics.dir/rigidbody_world.c.i"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sargarass/Cycles/geopattern/source/blender/editors/physics/rigidbody_world.c > CMakeFiles/bf_editor_physics.dir/rigidbody_world.c.i

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/rigidbody_world.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bf_editor_physics.dir/rigidbody_world.c.s"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sargarass/Cycles/geopattern/source/blender/editors/physics/rigidbody_world.c -o CMakeFiles/bf_editor_physics.dir/rigidbody_world.c.s

# Object files for target bf_editor_physics
bf_editor_physics_OBJECTS = \
"CMakeFiles/bf_editor_physics.dir/dynamicpaint_ops.c.o" \
"CMakeFiles/bf_editor_physics.dir/particle_boids.c.o" \
"CMakeFiles/bf_editor_physics.dir/particle_edit.c.o" \
"CMakeFiles/bf_editor_physics.dir/particle_object.c.o" \
"CMakeFiles/bf_editor_physics.dir/physics_fluid.c.o" \
"CMakeFiles/bf_editor_physics.dir/physics_ops.c.o" \
"CMakeFiles/bf_editor_physics.dir/physics_pointcache.c.o" \
"CMakeFiles/bf_editor_physics.dir/rigidbody_constraint.c.o" \
"CMakeFiles/bf_editor_physics.dir/rigidbody_object.c.o" \
"CMakeFiles/bf_editor_physics.dir/rigidbody_world.c.o"

# External object files for target bf_editor_physics
bf_editor_physics_EXTERNAL_OBJECTS =

lib/libbf_editor_physics.a: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/dynamicpaint_ops.c.o
lib/libbf_editor_physics.a: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/particle_boids.c.o
lib/libbf_editor_physics.a: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/particle_edit.c.o
lib/libbf_editor_physics.a: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/particle_object.c.o
lib/libbf_editor_physics.a: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/physics_fluid.c.o
lib/libbf_editor_physics.a: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/physics_ops.c.o
lib/libbf_editor_physics.a: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/physics_pointcache.c.o
lib/libbf_editor_physics.a: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/rigidbody_constraint.c.o
lib/libbf_editor_physics.a: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/rigidbody_object.c.o
lib/libbf_editor_physics.a: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/rigidbody_world.c.o
lib/libbf_editor_physics.a: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/build.make
lib/libbf_editor_physics.a: source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sargarass/Cycles/geopattern/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking C static library ../../../../lib/libbf_editor_physics.a"
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && $(CMAKE_COMMAND) -P CMakeFiles/bf_editor_physics.dir/cmake_clean_target.cmake
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bf_editor_physics.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/build: lib/libbf_editor_physics.a

.PHONY : source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/build

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/clean:
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics && $(CMAKE_COMMAND) -P CMakeFiles/bf_editor_physics.dir/cmake_clean.cmake
.PHONY : source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/clean

source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/depend:
	cd /home/sargarass/Cycles/geopattern/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sargarass/Cycles/geopattern /home/sargarass/Cycles/geopattern/source/blender/editors/physics /home/sargarass/Cycles/geopattern/cmake-build-debug /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics /home/sargarass/Cycles/geopattern/cmake-build-debug/source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : source/blender/editors/physics/CMakeFiles/bf_editor_physics.dir/depend

