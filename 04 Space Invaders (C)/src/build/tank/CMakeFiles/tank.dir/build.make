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
include tank/CMakeFiles/tank.dir/depend.make

# Include the progress variables for this target.
include tank/CMakeFiles/tank.dir/progress.make

# Include the compile flags for this target's objects.
include tank/CMakeFiles/tank.dir/flags.make

tank/CMakeFiles/tank.dir/tank.c.o: tank/CMakeFiles/tank.dir/flags.make
tank/CMakeFiles/tank.dir/tank.c.o: ../tank/tank.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xilinx/lab6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tank/CMakeFiles/tank.dir/tank.c.o"
	cd /home/xilinx/lab6/build/tank && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tank.dir/tank.c.o   -c /home/xilinx/lab6/tank/tank.c

tank/CMakeFiles/tank.dir/tank.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tank.dir/tank.c.i"
	cd /home/xilinx/lab6/build/tank && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/xilinx/lab6/tank/tank.c > CMakeFiles/tank.dir/tank.c.i

tank/CMakeFiles/tank.dir/tank.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tank.dir/tank.c.s"
	cd /home/xilinx/lab6/build/tank && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/xilinx/lab6/tank/tank.c -o CMakeFiles/tank.dir/tank.c.s

tank/CMakeFiles/tank.dir/tank.c.o.requires:

.PHONY : tank/CMakeFiles/tank.dir/tank.c.o.requires

tank/CMakeFiles/tank.dir/tank.c.o.provides: tank/CMakeFiles/tank.dir/tank.c.o.requires
	$(MAKE) -f tank/CMakeFiles/tank.dir/build.make tank/CMakeFiles/tank.dir/tank.c.o.provides.build
.PHONY : tank/CMakeFiles/tank.dir/tank.c.o.provides

tank/CMakeFiles/tank.dir/tank.c.o.provides.build: tank/CMakeFiles/tank.dir/tank.c.o


# Object files for target tank
tank_OBJECTS = \
"CMakeFiles/tank.dir/tank.c.o"

# External object files for target tank
tank_EXTERNAL_OBJECTS =

tank/libtank.a: tank/CMakeFiles/tank.dir/tank.c.o
tank/libtank.a: tank/CMakeFiles/tank.dir/build.make
tank/libtank.a: tank/CMakeFiles/tank.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xilinx/lab6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libtank.a"
	cd /home/xilinx/lab6/build/tank && $(CMAKE_COMMAND) -P CMakeFiles/tank.dir/cmake_clean_target.cmake
	cd /home/xilinx/lab6/build/tank && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tank.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tank/CMakeFiles/tank.dir/build: tank/libtank.a

.PHONY : tank/CMakeFiles/tank.dir/build

tank/CMakeFiles/tank.dir/requires: tank/CMakeFiles/tank.dir/tank.c.o.requires

.PHONY : tank/CMakeFiles/tank.dir/requires

tank/CMakeFiles/tank.dir/clean:
	cd /home/xilinx/lab6/build/tank && $(CMAKE_COMMAND) -P CMakeFiles/tank.dir/cmake_clean.cmake
.PHONY : tank/CMakeFiles/tank.dir/clean

tank/CMakeFiles/tank.dir/depend:
	cd /home/xilinx/lab6/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xilinx/lab6 /home/xilinx/lab6/tank /home/xilinx/lab6/build /home/xilinx/lab6/build/tank /home/xilinx/lab6/build/tank/CMakeFiles/tank.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tank/CMakeFiles/tank.dir/depend

