# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/starlight/Desktop/Yandex.Disk/Yandex Data Science School/C++/jpeg-decoder"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/starlight/Desktop/Yandex.Disk/Yandex Data Science School/C++/jpeg-decoder/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/my_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/my_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/my_test.dir/flags.make

CMakeFiles/my_test.dir/my_test.cpp.o: CMakeFiles/my_test.dir/flags.make
CMakeFiles/my_test.dir/my_test.cpp.o: ../my_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/starlight/Desktop/Yandex.Disk/Yandex Data Science School/C++/jpeg-decoder/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/my_test.dir/my_test.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/my_test.dir/my_test.cpp.o -c "/Users/starlight/Desktop/Yandex.Disk/Yandex Data Science School/C++/jpeg-decoder/my_test.cpp"

CMakeFiles/my_test.dir/my_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/my_test.dir/my_test.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/starlight/Desktop/Yandex.Disk/Yandex Data Science School/C++/jpeg-decoder/my_test.cpp" > CMakeFiles/my_test.dir/my_test.cpp.i

CMakeFiles/my_test.dir/my_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/my_test.dir/my_test.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/starlight/Desktop/Yandex.Disk/Yandex Data Science School/C++/jpeg-decoder/my_test.cpp" -o CMakeFiles/my_test.dir/my_test.cpp.s

CMakeFiles/my_test.dir/my_test.cpp.o.requires:

.PHONY : CMakeFiles/my_test.dir/my_test.cpp.o.requires

CMakeFiles/my_test.dir/my_test.cpp.o.provides: CMakeFiles/my_test.dir/my_test.cpp.o.requires
	$(MAKE) -f CMakeFiles/my_test.dir/build.make CMakeFiles/my_test.dir/my_test.cpp.o.provides.build
.PHONY : CMakeFiles/my_test.dir/my_test.cpp.o.provides

CMakeFiles/my_test.dir/my_test.cpp.o.provides.build: CMakeFiles/my_test.dir/my_test.cpp.o


# Object files for target my_test
my_test_OBJECTS = \
"CMakeFiles/my_test.dir/my_test.cpp.o"

# External object files for target my_test
my_test_EXTERNAL_OBJECTS =

my_test: CMakeFiles/my_test.dir/my_test.cpp.o
my_test: CMakeFiles/my_test.dir/build.make
my_test: libtest-lib.dylib
my_test: libdecoder-lib.dylib
my_test: CMakeFiles/my_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/starlight/Desktop/Yandex.Disk/Yandex Data Science School/C++/jpeg-decoder/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable my_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/my_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/my_test.dir/build: my_test

.PHONY : CMakeFiles/my_test.dir/build

CMakeFiles/my_test.dir/requires: CMakeFiles/my_test.dir/my_test.cpp.o.requires

.PHONY : CMakeFiles/my_test.dir/requires

CMakeFiles/my_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/my_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/my_test.dir/clean

CMakeFiles/my_test.dir/depend:
	cd "/Users/starlight/Desktop/Yandex.Disk/Yandex Data Science School/C++/jpeg-decoder/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/starlight/Desktop/Yandex.Disk/Yandex Data Science School/C++/jpeg-decoder" "/Users/starlight/Desktop/Yandex.Disk/Yandex Data Science School/C++/jpeg-decoder" "/Users/starlight/Desktop/Yandex.Disk/Yandex Data Science School/C++/jpeg-decoder/cmake-build-debug" "/Users/starlight/Desktop/Yandex.Disk/Yandex Data Science School/C++/jpeg-decoder/cmake-build-debug" "/Users/starlight/Desktop/Yandex.Disk/Yandex Data Science School/C++/jpeg-decoder/cmake-build-debug/CMakeFiles/my_test.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/my_test.dir/depend
