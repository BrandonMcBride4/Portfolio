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
include fit/CMakeFiles/fit.dir/depend.make

# Include the progress variables for this target.
include fit/CMakeFiles/fit.dir/progress.make

# Include the compile flags for this target's objects.
include fit/CMakeFiles/fit.dir/flags.make

fit/CMakeFiles/fit.dir/fit.c.o: fit/CMakeFiles/fit.dir/flags.make
fit/CMakeFiles/fit.dir/fit.c.o: ../fit/fit.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xilinx/lab6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object fit/CMakeFiles/fit.dir/fit.c.o"
	cd /home/xilinx/lab6/build/fit && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/fit.dir/fit.c.o   -c /home/xilinx/lab6/fit/fit.c

fit/CMakeFiles/fit.dir/fit.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/fit.dir/fit.c.i"
	cd /home/xilinx/lab6/build/fit && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/xilinx/lab6/fit/fit.c > CMakeFiles/fit.dir/fit.c.i

fit/CMakeFiles/fit.dir/fit.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/fit.dir/fit.c.s"
	cd /home/xilinx/lab6/build/fit && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/xilinx/lab6/fit/fit.c -o CMakeFiles/fit.dir/fit.c.s

fit/CMakeFiles/fit.dir/fit.c.o.requires:

.PHONY : fit/CMakeFiles/fit.dir/fit.c.o.requires

fit/CMakeFiles/fit.dir/fit.c.o.provides: fit/CMakeFiles/fit.dir/fit.c.o.requires
	$(MAKE) -f fit/CMakeFiles/fit.dir/build.make fit/CMakeFiles/fit.dir/fit.c.o.provides.build
.PHONY : fit/CMakeFiles/fit.dir/fit.c.o.provides

fit/CMakeFiles/fit.dir/fit.c.o.provides.build: fit/CMakeFiles/fit.dir/fit.c.o


# Object files for target fit
fit_OBJECTS = \
"CMakeFiles/fit.dir/fit.c.o"

# External object files for target fit
fit_EXTERNAL_OBJECTS =

fit/libfit.a: fit/CMakeFiles/fit.dir/fit.c.o
fit/libfit.a: fit/CMakeFiles/fit.dir/build.make
fit/libfit.a: fit/CMakeFiles/fit.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xilinx/lab6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libfit.a"
	cd /home/xilinx/lab6/build/fit && $(CMAKE_COMMAND) -P CMakeFiles/fit.dir/cmake_clean_target.cmake
	cd /home/xilinx/lab6/build/fit && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fit.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
fit/CMakeFiles/fit.dir/build: fit/libfit.a

.PHONY : fit/CMakeFiles/fit.dir/build

fit/CMakeFiles/fit.dir/requires: fit/CMakeFiles/fit.dir/fit.c.o.requires

.PHONY : fit/CMakeFiles/fit.dir/requires

fit/CMakeFiles/fit.dir/clean:
	cd /home/xilinx/lab6/build/fit && $(CMAKE_COMMAND) -P CMakeFiles/fit.dir/cmake_clean.cmake
.PHONY : fit/CMakeFiles/fit.dir/clean

fit/CMakeFiles/fit.dir/depend:
	cd /home/xilinx/lab6/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xilinx/lab6 /home/xilinx/lab6/fit /home/xilinx/lab6/build /home/xilinx/lab6/build/fit /home/xilinx/lab6/build/fit/CMakeFiles/fit.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : fit/CMakeFiles/fit.dir/depend

