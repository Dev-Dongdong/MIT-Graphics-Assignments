# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.21

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = D:\CodingEnvironment\CMake\bin\cmake.exe

# The command to remove a file.
RM = D:\CodingEnvironment\CMake\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "D:\Coding\MIT Graphics\Assignment2"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "D:\Coding\MIT Graphics\Assignment2\build"

# Include any dependencies generated for this target.
include CMakeFiles/raytracer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/raytracer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/raytracer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/raytracer.dir/flags.make

CMakeFiles/raytracer.dir/image.cpp.obj: CMakeFiles/raytracer.dir/flags.make
CMakeFiles/raytracer.dir/image.cpp.obj: ../image.cpp
CMakeFiles/raytracer.dir/image.cpp.obj: CMakeFiles/raytracer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\Coding\MIT Graphics\Assignment2\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/raytracer.dir/image.cpp.obj"
	D:\CodingEnvironment\TDM-GCC\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/raytracer.dir/image.cpp.obj -MF CMakeFiles\raytracer.dir\image.cpp.obj.d -o CMakeFiles\raytracer.dir\image.cpp.obj -c "D:\Coding\MIT Graphics\Assignment2\image.cpp"

CMakeFiles/raytracer.dir/image.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/raytracer.dir/image.cpp.i"
	D:\CodingEnvironment\TDM-GCC\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\Coding\MIT Graphics\Assignment2\image.cpp" > CMakeFiles\raytracer.dir\image.cpp.i

CMakeFiles/raytracer.dir/image.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/raytracer.dir/image.cpp.s"
	D:\CodingEnvironment\TDM-GCC\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\Coding\MIT Graphics\Assignment2\image.cpp" -o CMakeFiles\raytracer.dir\image.cpp.s

CMakeFiles/raytracer.dir/main.cpp.obj: CMakeFiles/raytracer.dir/flags.make
CMakeFiles/raytracer.dir/main.cpp.obj: ../main.cpp
CMakeFiles/raytracer.dir/main.cpp.obj: CMakeFiles/raytracer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\Coding\MIT Graphics\Assignment2\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/raytracer.dir/main.cpp.obj"
	D:\CodingEnvironment\TDM-GCC\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/raytracer.dir/main.cpp.obj -MF CMakeFiles\raytracer.dir\main.cpp.obj.d -o CMakeFiles\raytracer.dir\main.cpp.obj -c "D:\Coding\MIT Graphics\Assignment2\main.cpp"

CMakeFiles/raytracer.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/raytracer.dir/main.cpp.i"
	D:\CodingEnvironment\TDM-GCC\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\Coding\MIT Graphics\Assignment2\main.cpp" > CMakeFiles\raytracer.dir\main.cpp.i

CMakeFiles/raytracer.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/raytracer.dir/main.cpp.s"
	D:\CodingEnvironment\TDM-GCC\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\Coding\MIT Graphics\Assignment2\main.cpp" -o CMakeFiles\raytracer.dir\main.cpp.s

CMakeFiles/raytracer.dir/matrix.cpp.obj: CMakeFiles/raytracer.dir/flags.make
CMakeFiles/raytracer.dir/matrix.cpp.obj: ../matrix.cpp
CMakeFiles/raytracer.dir/matrix.cpp.obj: CMakeFiles/raytracer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\Coding\MIT Graphics\Assignment2\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/raytracer.dir/matrix.cpp.obj"
	D:\CodingEnvironment\TDM-GCC\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/raytracer.dir/matrix.cpp.obj -MF CMakeFiles\raytracer.dir\matrix.cpp.obj.d -o CMakeFiles\raytracer.dir\matrix.cpp.obj -c "D:\Coding\MIT Graphics\Assignment2\matrix.cpp"

CMakeFiles/raytracer.dir/matrix.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/raytracer.dir/matrix.cpp.i"
	D:\CodingEnvironment\TDM-GCC\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\Coding\MIT Graphics\Assignment2\matrix.cpp" > CMakeFiles\raytracer.dir\matrix.cpp.i

CMakeFiles/raytracer.dir/matrix.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/raytracer.dir/matrix.cpp.s"
	D:\CodingEnvironment\TDM-GCC\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\Coding\MIT Graphics\Assignment2\matrix.cpp" -o CMakeFiles\raytracer.dir\matrix.cpp.s

CMakeFiles/raytracer.dir/scene_parser.cpp.obj: CMakeFiles/raytracer.dir/flags.make
CMakeFiles/raytracer.dir/scene_parser.cpp.obj: ../scene_parser.cpp
CMakeFiles/raytracer.dir/scene_parser.cpp.obj: CMakeFiles/raytracer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\Coding\MIT Graphics\Assignment2\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/raytracer.dir/scene_parser.cpp.obj"
	D:\CodingEnvironment\TDM-GCC\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/raytracer.dir/scene_parser.cpp.obj -MF CMakeFiles\raytracer.dir\scene_parser.cpp.obj.d -o CMakeFiles\raytracer.dir\scene_parser.cpp.obj -c "D:\Coding\MIT Graphics\Assignment2\scene_parser.cpp"

CMakeFiles/raytracer.dir/scene_parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/raytracer.dir/scene_parser.cpp.i"
	D:\CodingEnvironment\TDM-GCC\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\Coding\MIT Graphics\Assignment2\scene_parser.cpp" > CMakeFiles\raytracer.dir\scene_parser.cpp.i

CMakeFiles/raytracer.dir/scene_parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/raytracer.dir/scene_parser.cpp.s"
	D:\CodingEnvironment\TDM-GCC\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\Coding\MIT Graphics\Assignment2\scene_parser.cpp" -o CMakeFiles\raytracer.dir\scene_parser.cpp.s

# Object files for target raytracer
raytracer_OBJECTS = \
"CMakeFiles/raytracer.dir/image.cpp.obj" \
"CMakeFiles/raytracer.dir/main.cpp.obj" \
"CMakeFiles/raytracer.dir/matrix.cpp.obj" \
"CMakeFiles/raytracer.dir/scene_parser.cpp.obj"

# External object files for target raytracer
raytracer_EXTERNAL_OBJECTS =

raytracer.exe: CMakeFiles/raytracer.dir/image.cpp.obj
raytracer.exe: CMakeFiles/raytracer.dir/main.cpp.obj
raytracer.exe: CMakeFiles/raytracer.dir/matrix.cpp.obj
raytracer.exe: CMakeFiles/raytracer.dir/scene_parser.cpp.obj
raytracer.exe: CMakeFiles/raytracer.dir/build.make
raytracer.exe: CMakeFiles/raytracer.dir/linklibs.rsp
raytracer.exe: CMakeFiles/raytracer.dir/objects1.rsp
raytracer.exe: CMakeFiles/raytracer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="D:\Coding\MIT Graphics\Assignment2\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable raytracer.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\raytracer.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/raytracer.dir/build: raytracer.exe
.PHONY : CMakeFiles/raytracer.dir/build

CMakeFiles/raytracer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\raytracer.dir\cmake_clean.cmake
.PHONY : CMakeFiles/raytracer.dir/clean

CMakeFiles/raytracer.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "D:\Coding\MIT Graphics\Assignment2" "D:\Coding\MIT Graphics\Assignment2" "D:\Coding\MIT Graphics\Assignment2\build" "D:\Coding\MIT Graphics\Assignment2\build" "D:\Coding\MIT Graphics\Assignment2\build\CMakeFiles\raytracer.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/raytracer.dir/depend

