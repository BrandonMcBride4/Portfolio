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
CMAKE_SOURCE_DIR = /home/xilinx/lab2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xilinx/lab2

# Include any dependencies generated for this target.
include btns/CMakeFiles/btns.dir/depend.make

# Include the progress variables for this target.
include btns/CMakeFiles/btns.dir/progress.make

# Include the compile flags for this target's objects.
include btns/CMakeFiles/btns.dir/flags.make

btns/CMakeFiles/btns.dir/btns.c.o: btns/CMakeFiles/btns.dir/flags.make
btns/CMakeFiles/btns.dir/btns.c.o: btns/btns.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xilinx/lab2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object btns/CMakeFiles/btns.dir/btns.c.o"
	cd /home/xilinx/lab2/btns && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/btns.dir/btns.c.o   -c /home/xilinx/lab2/btns/btns.c

btns/CMakeFiles/btns.dir/btns.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/btns.dir/btns.c.i"
	cd /home/xilinx/lab2/btns && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/xilinx/lab2/btns/btns.c > CMakeFiles/btns.dir/btns.c.i

btns/CMakeFiles/btns.dir/btns.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/btns.dir/btns.c.s"
	cd /home/xilinx/lab2/btns && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/xilinx/lab2/btns/btns.c -o CMakeFiles/btns.dir/btns.c.s

btns/CMakeFiles/btns.dir/btns.c.o.requires:

.PHONY : btns/CMakeFiles/btns.dir/btns.c.o.requires

btns/CMakeFiles/btns.dir/btns.c.o.provides: btns/CMakeFiles/btns.dir/btns.c.o.requires
	$(MAKE) -f btns/CMakeFiles/btns.dir/build.make btns/CMakeFiles/btns.dir/btns.c.o.provides.build
.PHONY : btns/CMakeFiles/btns.dir/btns.c.o.provides

btns/CMakeFiles/btns.dir/btns.c.o.provides.build: btns/CMakeFiles/btns.dir/btns.c.o


# Object files for target btns
btns_OBJECTS = \
"CMakeFiles/btns.dir/btns.c.o"

# External object files for target btns
btns_EXTERNAL_OBJECTS =

btns/libbtns.a: btns/CMakeFiles/btns.dir/btns.c.o
btns/libbtns.a: btns/CMakeFiles/btns.dir/build.make
btns/libbtns.a: btns/CMakeFiles/btns.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xilinx/lab2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libbtns.a"
	cd /home/xilinx/lab2/btns && $(CMAKE_COMMAND) -P CMakeFiles/btns.dir/cmake_clean_target.cmake
	cd /home/xilinx/lab2/btns && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/btns.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
btns/CMakeFiles/btns.dir/build: btns/libbtns.a

.PHONY : btns/CMakeFiles/btns.dir/build

btns/CMakeFiles/btns.dir/requires: btns/CMakeFiles/btns.dir/btns.c.o.requires

.PHONY : btns/CMakeFiles/btns.dir/requires

btns/CMakeFiles/btns.dir/clean:
	cd /home/xilinx/lab2/btns && $(CMAKE_COMMAND) -P CMakeFiles/btns.dir/cmake_clean.cmake
.PHONY : btns/CMakeFiles/btns.dir/clean

btns/CMakeFiles/btns.dir/depend:
	cd /home/xilinx/lab2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xilinx/lab2 /home/xilinx/lab2/btns /home/xilinx/lab2 /home/xilinx/lab2/btns /home/xilinx/lab2/btns/CMakeFiles/btns.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : btns/CMakeFiles/btns.dir/depend
