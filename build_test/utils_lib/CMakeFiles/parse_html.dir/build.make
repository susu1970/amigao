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
CMAKE_SOURCE_DIR = /home/ydssbbnc/do/document/source/prj/amigao

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ydssbbnc/do/document/source/prj/amigao/build_test

# Include any dependencies generated for this target.
include utils_lib/CMakeFiles/parse_html.dir/depend.make

# Include the progress variables for this target.
include utils_lib/CMakeFiles/parse_html.dir/progress.make

# Include the compile flags for this target's objects.
include utils_lib/CMakeFiles/parse_html.dir/flags.make

utils_lib/CMakeFiles/parse_html.dir/parse_html.cc.o: utils_lib/CMakeFiles/parse_html.dir/flags.make
utils_lib/CMakeFiles/parse_html.dir/parse_html.cc.o: ../src/utils/parse_html.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ydssbbnc/do/document/source/prj/amigao/build_test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object utils_lib/CMakeFiles/parse_html.dir/parse_html.cc.o"
	cd /home/ydssbbnc/do/document/source/prj/amigao/build_test/utils_lib && /bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/parse_html.dir/parse_html.cc.o -c /home/ydssbbnc/do/document/source/prj/amigao/src/utils/parse_html.cc

utils_lib/CMakeFiles/parse_html.dir/parse_html.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/parse_html.dir/parse_html.cc.i"
	cd /home/ydssbbnc/do/document/source/prj/amigao/build_test/utils_lib && /bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ydssbbnc/do/document/source/prj/amigao/src/utils/parse_html.cc > CMakeFiles/parse_html.dir/parse_html.cc.i

utils_lib/CMakeFiles/parse_html.dir/parse_html.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/parse_html.dir/parse_html.cc.s"
	cd /home/ydssbbnc/do/document/source/prj/amigao/build_test/utils_lib && /bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ydssbbnc/do/document/source/prj/amigao/src/utils/parse_html.cc -o CMakeFiles/parse_html.dir/parse_html.cc.s

# Object files for target parse_html
parse_html_OBJECTS = \
"CMakeFiles/parse_html.dir/parse_html.cc.o"

# External object files for target parse_html
parse_html_EXTERNAL_OBJECTS =

utils_lib/libparse_html.a: utils_lib/CMakeFiles/parse_html.dir/parse_html.cc.o
utils_lib/libparse_html.a: utils_lib/CMakeFiles/parse_html.dir/build.make
utils_lib/libparse_html.a: utils_lib/CMakeFiles/parse_html.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ydssbbnc/do/document/source/prj/amigao/build_test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libparse_html.a"
	cd /home/ydssbbnc/do/document/source/prj/amigao/build_test/utils_lib && $(CMAKE_COMMAND) -P CMakeFiles/parse_html.dir/cmake_clean_target.cmake
	cd /home/ydssbbnc/do/document/source/prj/amigao/build_test/utils_lib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/parse_html.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
utils_lib/CMakeFiles/parse_html.dir/build: utils_lib/libparse_html.a

.PHONY : utils_lib/CMakeFiles/parse_html.dir/build

utils_lib/CMakeFiles/parse_html.dir/clean:
	cd /home/ydssbbnc/do/document/source/prj/amigao/build_test/utils_lib && $(CMAKE_COMMAND) -P CMakeFiles/parse_html.dir/cmake_clean.cmake
.PHONY : utils_lib/CMakeFiles/parse_html.dir/clean

utils_lib/CMakeFiles/parse_html.dir/depend:
	cd /home/ydssbbnc/do/document/source/prj/amigao/build_test && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ydssbbnc/do/document/source/prj/amigao /home/ydssbbnc/do/document/source/prj/amigao/src/utils /home/ydssbbnc/do/document/source/prj/amigao/build_test /home/ydssbbnc/do/document/source/prj/amigao/build_test/utils_lib /home/ydssbbnc/do/document/source/prj/amigao/build_test/utils_lib/CMakeFiles/parse_html.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : utils_lib/CMakeFiles/parse_html.dir/depend
