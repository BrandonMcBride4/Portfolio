# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/xilinx/lab6

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xilinx/lab6/build

# Include any dependencies generated for this target.
include tank_bullet/CMakeFiles/tank_bullet.dir/depend.make

# Include the progress variables for this target.
include tank_bullet/CMakeFiles/tank_bullet.dir/progress.make

# Include the compile flags for this target's objects.
include tank_bullet/CMakeFiles/tank_bullet.dir/flags.make

tank_bullet/CMakeFiles/tank_bullet.dir/tank_bullet.c.o: tank_bullet/CMakeFiles/tank_bullet.dir/flags.make
tank_bullet/CMakeFiles/tank_bullet.dir/tank_bullet.c.o: ../tank_bullet/tank_bullet.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xilinx/lab6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tank_bullet/CMakeFiles/tank_bullet.dir/tank_bullet.c.o"
	cd /home/xilinx/lab6/build/tank_bullet && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tank_bullet.dir/tank_bullet.c.o   -c /home/xilinx/lab6/tank_bullet/tank_bullet.c

tank_bullet/CMakeFiles/tank_bullet.dir/tank_bullet.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tank_bullet.dir/tank_bullet.c.i"
	cd /home/xilinx/lab6/build/tank_bullet && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/xilinx/lab6/tank_bullet/tank_bullet.c > CMakeFiles/tank_bullet.dir/tank_bullet.c.i

tank_bullet/CMakeFiles/tank_bullet.dir/tank_bullet.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tank_bullet.dir/tank_bullet.c.s"
	cd /home/xilinx/lab6/build/tank_bullet && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/xilinx/lab6/tank_bullet/tank_bullet.c -o CMakeFiles/tank_bullet.dir/tank_bullet.c.s

tank_bullet/CMakeFiles/tank_bullet.dir/tank_bullet.c.o.requires:

.PHONY : tank_bullet/CMakeFiles/tank_bullet.dir/tank_bullet.c.o.requires

tank_bullet/CMakeFiles/tank_bullet.dir/tank_bullet.c.o.provides: tank_bullet/CMakeFiles/tank_bullet.dir/tank_bullet.c.o.requires
	$(MAKE) -f tank_bullet/CMakeFiles/tank_bullet.dir/build.make tank_bullet/CMakeFiles/tank_bullet.dir/tank_bullet.c.o.provides.build
.PHONY : tank_bullet/CMakeFiles/tank_bullet.dir/tank_bullet.c.o.provides

tank_bullet/CMakeFiles/tank_bullet.dir/tank_bullet.c.o.provides.build: tank_bullet/CMakeFiles/tank_bullet.dir/tank_bullet.c.o


# Object files for target tank_bullet
tank_bullet_OBJECTS = \
"CMakeFiles/tank_bullet.dir/tank_bullet.c.o"

# External object files for target tank_bullet
tank_bullet_EXTERNAL_OBJECTS =

tank_bullet/libtank_bullet.a: tank_bullet/CMakeFiles/tank_bullet.dir/tank_bullet.c.o
tank_bullet/libtank_bullet.a: tank_bullet/CMakeFiles/tank_bullet.dir/build.make
tank_bullet/libtank_bullet.a: tank_bullet/CMakeFiles/tank_bullet.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xilinx/lab6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libtank_bullet.a"
	cd /home/xilinx/lab6/build/tank_bullet && $(CMAKE_COMMAND) -P CMakeFiles/tank_bullet.dir/cmake_clean_target.cmake
	cd /home/xilinx/lab6/build/tank_bullet && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tank_bullet.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tank_bullet/CMakeFiles/tank_bullet.dir/build: tank_bullet/libtank_bullet.a

.PHONY : tank_bullet/CMakeFiles/tank_bullet.dir/build

tank_bullet/CMakeFiles/tank_bullet.dir/requires: tank_bullet/CMakeFiles/tank_bullet.dir/tank_bullet.c.o.requires

.PHONY : tank_bullet/CMakeFiles/tank_bullet.dir/requires

tank_bullet/CMakeFiles/tank_bullet.dir/clean:
	cd /home/xilinx/lab6/build/tank_bullet && $(CMAKE_COMMAND) -P CMakeFiles/tank_bullet.dir/cmake_clean.cmake
.PHONY : tank_bullet/CMakeFiles/tank_bullet.dir/clean

tank_bullet/CMakeFiles/tank_bullet.dir/depend:
	cd /home/xilinx/lab6/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xilinx/lab6 /home/xilinx/lab6/tank_bullet /home/xilinx/lab6/build /home/xilinx/lab6/build/tank_bullet /home/xilinx/lab6/build/tank_bullet/CMakeFiles/tank_bullet.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tank_bullet/CMakeFiles/tank_bullet.dir/depend

