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
CMAKE_SOURCE_DIR = /home/ganidhu/Desktop/vulkan-render-engine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ganidhu/Desktop/vulkan-render-engine

# Include any dependencies generated for this target.
include CMakeFiles/cmake-good.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/cmake-good.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cmake-good.dir/flags.make

CMakeFiles/cmake-good.dir/main.cpp.o: CMakeFiles/cmake-good.dir/flags.make
CMakeFiles/cmake-good.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ganidhu/Desktop/vulkan-render-engine/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/cmake-good.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cmake-good.dir/main.cpp.o -c /home/ganidhu/Desktop/vulkan-render-engine/main.cpp

CMakeFiles/cmake-good.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cmake-good.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ganidhu/Desktop/vulkan-render-engine/main.cpp > CMakeFiles/cmake-good.dir/main.cpp.i

CMakeFiles/cmake-good.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cmake-good.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ganidhu/Desktop/vulkan-render-engine/main.cpp -o CMakeFiles/cmake-good.dir/main.cpp.s

# Object files for target cmake-good
cmake__good_OBJECTS = \
"CMakeFiles/cmake-good.dir/main.cpp.o"

# External object files for target cmake-good
cmake__good_EXTERNAL_OBJECTS =

cmake-good: CMakeFiles/cmake-good.dir/main.cpp.o
cmake-good: CMakeFiles/cmake-good.dir/build.make
cmake-good: librequired_libs.a
cmake-good: CMakeFiles/cmake-good.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ganidhu/Desktop/vulkan-render-engine/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable cmake-good"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cmake-good.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cmake-good.dir/build: cmake-good

.PHONY : CMakeFiles/cmake-good.dir/build

CMakeFiles/cmake-good.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cmake-good.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cmake-good.dir/clean

CMakeFiles/cmake-good.dir/depend:
	cd /home/ganidhu/Desktop/vulkan-render-engine && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ganidhu/Desktop/vulkan-render-engine /home/ganidhu/Desktop/vulkan-render-engine /home/ganidhu/Desktop/vulkan-render-engine /home/ganidhu/Desktop/vulkan-render-engine /home/ganidhu/Desktop/vulkan-render-engine/CMakeFiles/cmake-good.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cmake-good.dir/depend

