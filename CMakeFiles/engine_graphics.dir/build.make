# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/ganidhu/Desktop/engine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ganidhu/Desktop/engine

# Include any dependencies generated for this target.
include CMakeFiles/engine_graphics.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/engine_graphics.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/engine_graphics.dir/flags.make

CMakeFiles/engine_graphics.dir/lib/engine_graphics.cpp.o: CMakeFiles/engine_graphics.dir/flags.make
CMakeFiles/engine_graphics.dir/lib/engine_graphics.cpp.o: lib/engine_graphics.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ganidhu/Desktop/engine/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/engine_graphics.dir/lib/engine_graphics.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/engine_graphics.dir/lib/engine_graphics.cpp.o -c /home/ganidhu/Desktop/engine/lib/engine_graphics.cpp

CMakeFiles/engine_graphics.dir/lib/engine_graphics.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/engine_graphics.dir/lib/engine_graphics.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ganidhu/Desktop/engine/lib/engine_graphics.cpp > CMakeFiles/engine_graphics.dir/lib/engine_graphics.cpp.i

CMakeFiles/engine_graphics.dir/lib/engine_graphics.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/engine_graphics.dir/lib/engine_graphics.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ganidhu/Desktop/engine/lib/engine_graphics.cpp -o CMakeFiles/engine_graphics.dir/lib/engine_graphics.cpp.s

# Object files for target engine_graphics
engine_graphics_OBJECTS = \
"CMakeFiles/engine_graphics.dir/lib/engine_graphics.cpp.o"

# External object files for target engine_graphics
engine_graphics_EXTERNAL_OBJECTS =

libengine_graphics.a: CMakeFiles/engine_graphics.dir/lib/engine_graphics.cpp.o
libengine_graphics.a: CMakeFiles/engine_graphics.dir/build.make
libengine_graphics.a: CMakeFiles/engine_graphics.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ganidhu/Desktop/engine/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libengine_graphics.a"
	$(CMAKE_COMMAND) -P CMakeFiles/engine_graphics.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/engine_graphics.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/engine_graphics.dir/build: libengine_graphics.a

.PHONY : CMakeFiles/engine_graphics.dir/build

CMakeFiles/engine_graphics.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/engine_graphics.dir/cmake_clean.cmake
.PHONY : CMakeFiles/engine_graphics.dir/clean

CMakeFiles/engine_graphics.dir/depend:
	cd /home/ganidhu/Desktop/engine && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ganidhu/Desktop/engine /home/ganidhu/Desktop/engine /home/ganidhu/Desktop/engine /home/ganidhu/Desktop/engine /home/ganidhu/Desktop/engine/CMakeFiles/engine_graphics.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/engine_graphics.dir/depend

