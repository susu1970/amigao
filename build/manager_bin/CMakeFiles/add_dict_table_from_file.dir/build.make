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
include manager_bin/CMakeFiles/add_dict_table_from_file.dir/depend.make

# Include the progress variables for this target.
include manager_bin/CMakeFiles/add_dict_table_from_file.dir/progress.make

# Include the compile flags for this target's objects.
include manager_bin/CMakeFiles/add_dict_table_from_file.dir/flags.make

manager_bin/CMakeFiles/add_dict_table_from_file.dir/add_dict_table_from_file.cc.o: manager_bin/CMakeFiles/add_dict_table_from_file.dir/flags.make
manager_bin/CMakeFiles/add_dict_table_from_file.dir/add_dict_table_from_file.cc.o: ../manager/add_dict_table_from_file.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ydssbbnc/do/document/source/prj/amigao/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object manager_bin/CMakeFiles/add_dict_table_from_file.dir/add_dict_table_from_file.cc.o"
	cd /home/ydssbbnc/do/document/source/prj/amigao/build/manager_bin && /bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/add_dict_table_from_file.dir/add_dict_table_from_file.cc.o -c /home/ydssbbnc/do/document/source/prj/amigao/manager/add_dict_table_from_file.cc

manager_bin/CMakeFiles/add_dict_table_from_file.dir/add_dict_table_from_file.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/add_dict_table_from_file.dir/add_dict_table_from_file.cc.i"
	cd /home/ydssbbnc/do/document/source/prj/amigao/build/manager_bin && /bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ydssbbnc/do/document/source/prj/amigao/manager/add_dict_table_from_file.cc > CMakeFiles/add_dict_table_from_file.dir/add_dict_table_from_file.cc.i

manager_bin/CMakeFiles/add_dict_table_from_file.dir/add_dict_table_from_file.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/add_dict_table_from_file.dir/add_dict_table_from_file.cc.s"
	cd /home/ydssbbnc/do/document/source/prj/amigao/build/manager_bin && /bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ydssbbnc/do/document/source/prj/amigao/manager/add_dict_table_from_file.cc -o CMakeFiles/add_dict_table_from_file.dir/add_dict_table_from_file.cc.s

# Object files for target add_dict_table_from_file
add_dict_table_from_file_OBJECTS = \
"CMakeFiles/add_dict_table_from_file.dir/add_dict_table_from_file.cc.o"

# External object files for target add_dict_table_from_file
add_dict_table_from_file_EXTERNAL_OBJECTS =

manager_bin/add_dict_table_from_file: manager_bin/CMakeFiles/add_dict_table_from_file.dir/add_dict_table_from_file.cc.o
manager_bin/add_dict_table_from_file: manager_bin/CMakeFiles/add_dict_table_from_file.dir/build.make
manager_bin/add_dict_table_from_file: utils_lib/libmaria_op.a
manager_bin/add_dict_table_from_file: utils_lib/libcrypt_string.a
manager_bin/add_dict_table_from_file: manager_bin/CMakeFiles/add_dict_table_from_file.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ydssbbnc/do/document/source/prj/amigao/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable add_dict_table_from_file"
	cd /home/ydssbbnc/do/document/source/prj/amigao/build/manager_bin && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/add_dict_table_from_file.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
manager_bin/CMakeFiles/add_dict_table_from_file.dir/build: manager_bin/add_dict_table_from_file

.PHONY : manager_bin/CMakeFiles/add_dict_table_from_file.dir/build

manager_bin/CMakeFiles/add_dict_table_from_file.dir/clean:
	cd /home/ydssbbnc/do/document/source/prj/amigao/build/manager_bin && $(CMAKE_COMMAND) -P CMakeFiles/add_dict_table_from_file.dir/cmake_clean.cmake
.PHONY : manager_bin/CMakeFiles/add_dict_table_from_file.dir/clean

manager_bin/CMakeFiles/add_dict_table_from_file.dir/depend:
	cd /home/ydssbbnc/do/document/source/prj/amigao/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ydssbbnc/do/document/source/prj/amigao /home/ydssbbnc/do/document/source/prj/amigao/manager /home/ydssbbnc/do/document/source/prj/amigao/build /home/ydssbbnc/do/document/source/prj/amigao/build/manager_bin /home/ydssbbnc/do/document/source/prj/amigao/build/manager_bin/CMakeFiles/add_dict_table_from_file.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : manager_bin/CMakeFiles/add_dict_table_from_file.dir/depend

