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
include aliens/CMakeFiles/aliens.dir/depend.make

# Include the progress variables for this target.
include aliens/CMakeFiles/aliens.dir/progress.make

# Include the compile flags for this target's objects.
include aliens/CMakeFiles/aliens.dir/flags.make

aliens/CMakeFiles/aliens.dir/aliens.c.o: aliens/CMakeFiles/aliens.dir/flags.make
aliens/CMakeFiles/aliens.dir/aliens.c.o: ../aliens/aliens.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xilinx/lab6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object aliens/CMakeFiles/aliens.dir/aliens.c.o"
	cd /home/xilinx/lab6/build/aliens && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/aliens.dir/aliens.c.o   -c /home/xilinx/lab6/aliens/aliens.c

aliens/CMakeFiles/aliens.dir/aliens.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/aliens.dir/aliens.c.i"
	cd /home/xilinx/lab6/build/aliens && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/xilinx/lab6/aliens/aliens.c > CMakeFiles/aliens.dir/aliens.c.i

aliens/CMakeFiles/aliens.dir/aliens.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/aliens.dir/aliens.c.s"
	cd /home/xilinx/lab6/build/aliens && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/xilinx/lab6/aliens/aliens.c -o CMakeFiles/aliens.dir/aliens.c.s

aliens/CMakeFiles/aliens.dir/aliens.c.o.requires:

.PHONY : aliens/CMakeFiles/aliens.dir/aliens.c.o.requires

aliens/CMakeFiles/aliens.dir/aliens.c.o.provides: aliens/CMakeFiles/aliens.dir/aliens.c.o.requires
	$(MAKE) -f aliens/CMakeFiles/aliens.dir/build.make aliens/CMakeFiles/aliens.dir/aliens.c.o.provides.build
.PHONY : aliens/CMakeFiles/aliens.dir/aliens.c.o.provides

aliens/CMakeFiles/aliens.dir/aliens.c.o.provides.build: aliens/CMakeFiles/aliens.dir/aliens.c.o


# Object files for target aliens
aliens_OBJECTS = \
"CMakeFiles/aliens.dir/aliens.c.o"

# External object files for target aliens
aliens_EXTERNAL_OBJECTS =

aliens/libaliens.a: aliens/CMakeFiles/aliens.dir/aliens.c.o
aliens/libaliens.a: aliens/CMakeFiles/aliens.dir/build.make
aliens/libaliens.a: aliens/CMakeFiles/aliens.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xilinx/lab6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libaliens.a"
	cd /home/xilinx/lab6/build/aliens && $(CMAKE_COMMAND) -P CMakeFiles/aliens.dir/cmake_clean_target.cmake
	cd /home/xilinx/lab6/build/aliens && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/aliens.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
aliens/CMakeFiles/aliens.dir/build: aliens/libaliens.a

.PHONY : aliens/CMakeFiles/aliens.dir/build

aliens/CMakeFiles/aliens.dir/requires: aliens/CMakeFiles/aliens.dir/aliens.c.o.requires

.PHONY : aliens/CMakeFiles/aliens.dir/requires

aliens/CMakeFiles/aliens.dir/clean:
	cd /home/xilinx/lab6/build/aliens && $(CMAKE_COMMAND) -P CMakeFiles/aliens.dir/cmake_clean.cmake
.PHONY : aliens/CMakeFiles/aliens.dir/clean

aliens/CMakeFiles/aliens.dir/depend:
	cd /home/xilinx/lab6/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xilinx/lab6 /home/xilinx/lab6/aliens /home/xilinx/lab6/build /home/xilinx/lab6/build/aliens /home/xilinx/lab6/build/aliens/CMakeFiles/aliens.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : aliens/CMakeFiles/aliens.dir/depend
