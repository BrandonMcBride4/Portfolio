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
include video/CMakeFiles/video.dir/depend.make

# Include the progress variables for this target.
include video/CMakeFiles/video.dir/progress.make

# Include the compile flags for this target's objects.
include video/CMakeFiles/video.dir/flags.make

video/CMakeFiles/video.dir/video.c.o: video/CMakeFiles/video.dir/flags.make
video/CMakeFiles/video.dir/video.c.o: ../video/video.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xilinx/lab6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object video/CMakeFiles/video.dir/video.c.o"
	cd /home/xilinx/lab6/build/video && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/video.dir/video.c.o   -c /home/xilinx/lab6/video/video.c

video/CMakeFiles/video.dir/video.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/video.dir/video.c.i"
	cd /home/xilinx/lab6/build/video && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/xilinx/lab6/video/video.c > CMakeFiles/video.dir/video.c.i

video/CMakeFiles/video.dir/video.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/video.dir/video.c.s"
	cd /home/xilinx/lab6/build/video && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/xilinx/lab6/video/video.c -o CMakeFiles/video.dir/video.c.s

video/CMakeFiles/video.dir/video.c.o.requires:

.PHONY : video/CMakeFiles/video.dir/video.c.o.requires

video/CMakeFiles/video.dir/video.c.o.provides: video/CMakeFiles/video.dir/video.c.o.requires
	$(MAKE) -f video/CMakeFiles/video.dir/build.make video/CMakeFiles/video.dir/video.c.o.provides.build
.PHONY : video/CMakeFiles/video.dir/video.c.o.provides

video/CMakeFiles/video.dir/video.c.o.provides.build: video/CMakeFiles/video.dir/video.c.o


# Object files for target video
video_OBJECTS = \
"CMakeFiles/video.dir/video.c.o"

# External object files for target video
video_EXTERNAL_OBJECTS =

video/libvideo.a: video/CMakeFiles/video.dir/video.c.o
video/libvideo.a: video/CMakeFiles/video.dir/build.make
video/libvideo.a: video/CMakeFiles/video.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xilinx/lab6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libvideo.a"
	cd /home/xilinx/lab6/build/video && $(CMAKE_COMMAND) -P CMakeFiles/video.dir/cmake_clean_target.cmake
	cd /home/xilinx/lab6/build/video && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/video.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
video/CMakeFiles/video.dir/build: video/libvideo.a

.PHONY : video/CMakeFiles/video.dir/build

video/CMakeFiles/video.dir/requires: video/CMakeFiles/video.dir/video.c.o.requires

.PHONY : video/CMakeFiles/video.dir/requires

video/CMakeFiles/video.dir/clean:
	cd /home/xilinx/lab6/build/video && $(CMAKE_COMMAND) -P CMakeFiles/video.dir/cmake_clean.cmake
.PHONY : video/CMakeFiles/video.dir/clean

video/CMakeFiles/video.dir/depend:
	cd /home/xilinx/lab6/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xilinx/lab6 /home/xilinx/lab6/video /home/xilinx/lab6/build /home/xilinx/lab6/build/video /home/xilinx/lab6/build/video/CMakeFiles/video.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : video/CMakeFiles/video.dir/depend

