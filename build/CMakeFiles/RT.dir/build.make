# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.28.1/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.28.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/gyeongjukim/my_git/raytracing

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/gyeongjukim/my_git/raytracing/build

# Include any dependencies generated for this target.
include CMakeFiles/RT.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/RT.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/RT.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RT.dir/flags.make

CMakeFiles/RT.dir/main.cpp.o: CMakeFiles/RT.dir/flags.make
CMakeFiles/RT.dir/main.cpp.o: /Users/gyeongjukim/my_git/raytracing/main.cpp
CMakeFiles/RT.dir/main.cpp.o: CMakeFiles/RT.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/gyeongjukim/my_git/raytracing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/RT.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RT.dir/main.cpp.o -MF CMakeFiles/RT.dir/main.cpp.o.d -o CMakeFiles/RT.dir/main.cpp.o -c /Users/gyeongjukim/my_git/raytracing/main.cpp

CMakeFiles/RT.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/RT.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/gyeongjukim/my_git/raytracing/main.cpp > CMakeFiles/RT.dir/main.cpp.i

CMakeFiles/RT.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/RT.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/gyeongjukim/my_git/raytracing/main.cpp -o CMakeFiles/RT.dir/main.cpp.s

# Object files for target RT
RT_OBJECTS = \
"CMakeFiles/RT.dir/main.cpp.o"

# External object files for target RT
RT_EXTERNAL_OBJECTS =

RT: CMakeFiles/RT.dir/main.cpp.o
RT: CMakeFiles/RT.dir/build.make
RT: CMakeFiles/RT.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/gyeongjukim/my_git/raytracing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable RT"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RT.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RT.dir/build: RT
.PHONY : CMakeFiles/RT.dir/build

CMakeFiles/RT.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RT.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RT.dir/clean

CMakeFiles/RT.dir/depend:
	cd /Users/gyeongjukim/my_git/raytracing/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/gyeongjukim/my_git/raytracing /Users/gyeongjukim/my_git/raytracing /Users/gyeongjukim/my_git/raytracing/build /Users/gyeongjukim/my_git/raytracing/build /Users/gyeongjukim/my_git/raytracing/build/CMakeFiles/RT.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/RT.dir/depend

