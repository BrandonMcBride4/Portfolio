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
include bunkers/CMakeFiles/bunkers.dir/depend.make

# Include the progress variables for this target.
include bunkers/CMakeFiles/bunkers.dir/progress.make

# Include the compile flags for this target's objects.
include bunkers/CMakeFiles/bunkers.dir/flags.make

bunkers/CMakeFiles/bunkers.dir/bunkers.c.o: bunkers/CMakeFiles/bunkers.dir/flags.make
bunkers/CMakeFiles/bunkers.dir/bunkers.c.o: ../bunkers/bunkers.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xilinx/lab6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object bunkers/CMakeFiles/bunkers.dir/bunkers.c.o"
	cd /home/xilinx/lab6/build/bunkers && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bunkers.dir/bunkers.c.o   -c /home/xilinx/lab6/bunkers/bunkers.c

bunkers/CMakeFiles/bunkers.dir/bunkers.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bunkers.dir/bunkers.c.i"
	cd /home/xilinx/lab6/build/bunkers && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/xilinx/lab6/bunkers/bunkers.c > CMakeFiles/bunkers.dir/bunkers.c.i

bunkers/CMakeFiles/bunkers.dir/bunkers.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bunkers.dir/bunkers.c.s"
	cd /home/xilinx/lab6/build/bunkers && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/xilinx/lab6/bunkers/bunkers.c -o CMakeFiles/bunkers.dir/bunkers.c.s

bunkers/CMakeFiles/bunkers.dir/bunkers.c.o.requires:

.PHONY : bunkers/CMakeFiles/bunkers.dir/bunkers.c.o.requires

bunkers/CMakeFiles/bunkers.dir/bunkers.c.o.provides: bunkers/CMakeFiles/bunkers.dir/bunkers.c.o.requires
	$(MAKE) -f bunkers/CMakeFiles/bunkers.dir/build.make bunkers/CMakeFiles/bunkers.dir/bunkers.c.o.provides.build
.PHONY : bunkers/CMakeFiles/bunkers.dir/bunkers.c.o.provides

bunkers/CMakeFiles/bunkers.dir/bunkers.c.o.provides.build: bunkers/CMakeFiles/bunkers.dir/bunkers.c.o


# Object files for target bunkers
bunkers_OBJECTS = \
"CMakeFiles/bunkers.dir/bunkers.c.o"

# External object files for target bunkers
bunkers_EXTERNAL_OBJECTS =

bunkers/libbunkers.a: bunkers/CMakeFiles/bunkers.dir/bunkers.c.o
bunkers/libbunkers.a: bunkers/CMakeFiles/bunkers.dir/build.make
bunkers/libbunkers.a: bunkers/CMakeFiles/bunkers.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xilinx/lab6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libbunkers.a"
	cd /home/xilinx/lab6/build/bunkers && $(CMAKE_COMMAND) -P CMakeFiles/bunkers.dir/cmake_clean_target.cmake
	cd /home/xilinx/lab6/build/bunkers && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bunkers.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
bunkers/CMakeFiles/bunkers.dir/build: bunkers/libbunkers.a

.PHONY : bunkers/CMakeFiles/bunkers.dir/build

bunkers/CMakeFiles/bunkers.dir/requires: bunkers/CMakeFiles/bunkers.dir/bunkers.c.o.requires

.PHONY : bunkers/CMakeFiles/bunkers.dir/requires

bunkers/CMakeFiles/bunkers.dir/clean:
	cd /home/xilinx/lab6/build/bunkers && $(CMAKE_COMMAND) -P CMakeFiles/bunkers.dir/cmake_clean.cmake
.PHONY : bunkers/CMakeFiles/bunkers.dir/clean

bunkers/CMakeFiles/bunkers.dir/depend:
	cd /home/xilinx/lab6/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xilinx/lab6 /home/xilinx/lab6/bunkers /home/xilinx/lab6/build /home/xilinx/lab6/build/bunkers /home/xilinx/lab6/build/bunkers/CMakeFiles/bunkers.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : bunkers/CMakeFiles/bunkers.dir/depend
