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
CMAKE_BINARY_DIR = /home/ydssbbnc/do/document/source/prj/amigao/build

# Include any dependencies generated for this target.
include bin/spider/CMakeFiles/master.dir/depend.make

# Include the progress variables for this target.
include bin/spider/CMakeFiles/master.dir/progress.make

# Include the compile flags for this target's objects.
include bin/spider/CMakeFiles/master.dir/flags.make

bin/spider/CMakeFiles/master.dir/master.cc.o: bin/spider/CMakeFiles/master.dir/flags.make
bin/spider/CMakeFiles/master.dir/master.cc.o: ../src/spider/master.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ydssbbnc/do/document/source/prj/amigao/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object bin/spider/CMakeFiles/master.dir/master.cc.o"
	cd /home/ydssbbnc/do/document/source/prj/amigao/build/bin/spider && /bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/master.dir/master.cc.o -c /home/ydssbbnc/do/document/source/prj/amigao/src/spider/master.cc

bin/spider/CMakeFiles/master.dir/master.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/master.dir/master.cc.i"
	cd /home/ydssbbnc/do/document/source/prj/amigao/build/bin/spider && /bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ydssbbnc/do/document/source/prj/amigao/src/spider/master.cc > CMakeFiles/master.dir/master.cc.i

bin/spider/CMakeFiles/master.dir/master.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/master.dir/master.cc.s"
	cd /home/ydssbbnc/do/document/source/prj/amigao/build/bin/spider && /bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ydssbbnc/do/document/source/prj/amigao/src/spider/master.cc -o CMakeFiles/master.dir/master.cc.s

# Object files for target master
master_OBJECTS = \
"CMakeFiles/master.dir/master.cc.o"

# External object files for target master
master_EXTERNAL_OBJECTS =

bin/spider/master: bin/spider/CMakeFiles/master.dir/master.cc.o
bin/spider/master: bin/spider/CMakeFiles/master.dir/build.make
bin/spider/master: utils_lib/libformat_url.a
bin/spider/master: utils_lib/libmaria_op.a
bin/spider/master: utils_lib/libcrypt_string.a
bin/spider/master: bin/spider/CMakeFiles/master.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ydssbbnc/do/document/source/prj/amigao/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable master"
	cd /home/ydssbbnc/do/document/source/prj/amigao/build/bin/spider && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/master.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
bin/spider/CMakeFiles/master.dir/build: bin/spider/master

.PHONY : bin/spider/CMakeFiles/master.dir/build

bin/spider/CMakeFiles/master.dir/clean:
	cd /home/ydssbbnc/do/document/source/prj/amigao/build/bin/spider && $(CMAKE_COMMAND) -P CMakeFiles/master.dir/cmake_clean.cmake
.PHONY : bin/spider/CMakeFiles/master.dir/clean

bin/spider/CMakeFiles/master.dir/depend:
	cd /home/ydssbbnc/do/document/source/prj/amigao/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ydssbbnc/do/document/source/prj/amigao /home/ydssbbnc/do/document/source/prj/amigao/src/spider /home/ydssbbnc/do/document/source/prj/amigao/build /home/ydssbbnc/do/document/source/prj/amigao/build/bin/spider /home/ydssbbnc/do/document/source/prj/amigao/build/bin/spider/CMakeFiles/master.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : bin/spider/CMakeFiles/master.dir/depend

