# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/aidle/cmpt433/work/assignment4-1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/aidle/cmpt433/work/assignment4-1/build

# Include any dependencies generated for this target.
include app/CMakeFiles/find_the_dot.dir/depend.make

# Include the progress variables for this target.
include app/CMakeFiles/find_the_dot.dir/progress.make

# Include the compile flags for this target's objects.
include app/CMakeFiles/find_the_dot.dir/flags.make

app/CMakeFiles/find_the_dot.dir/src/main.c.o: app/CMakeFiles/find_the_dot.dir/flags.make
app/CMakeFiles/find_the_dot.dir/src/main.c.o: ../app/src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aidle/cmpt433/work/assignment4-1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object app/CMakeFiles/find_the_dot.dir/src/main.c.o"
	cd /home/aidle/cmpt433/work/assignment4-1/build/app && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/find_the_dot.dir/src/main.c.o -c /home/aidle/cmpt433/work/assignment4-1/app/src/main.c

app/CMakeFiles/find_the_dot.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/find_the_dot.dir/src/main.c.i"
	cd /home/aidle/cmpt433/work/assignment4-1/build/app && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/aidle/cmpt433/work/assignment4-1/app/src/main.c > CMakeFiles/find_the_dot.dir/src/main.c.i

app/CMakeFiles/find_the_dot.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/find_the_dot.dir/src/main.c.s"
	cd /home/aidle/cmpt433/work/assignment4-1/build/app && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/aidle/cmpt433/work/assignment4-1/app/src/main.c -o CMakeFiles/find_the_dot.dir/src/main.c.s

app/CMakeFiles/find_the_dot.dir/src/sharedMem-Linux.c.o: app/CMakeFiles/find_the_dot.dir/flags.make
app/CMakeFiles/find_the_dot.dir/src/sharedMem-Linux.c.o: ../app/src/sharedMem-Linux.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aidle/cmpt433/work/assignment4-1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object app/CMakeFiles/find_the_dot.dir/src/sharedMem-Linux.c.o"
	cd /home/aidle/cmpt433/work/assignment4-1/build/app && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/find_the_dot.dir/src/sharedMem-Linux.c.o -c /home/aidle/cmpt433/work/assignment4-1/app/src/sharedMem-Linux.c

app/CMakeFiles/find_the_dot.dir/src/sharedMem-Linux.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/find_the_dot.dir/src/sharedMem-Linux.c.i"
	cd /home/aidle/cmpt433/work/assignment4-1/build/app && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/aidle/cmpt433/work/assignment4-1/app/src/sharedMem-Linux.c > CMakeFiles/find_the_dot.dir/src/sharedMem-Linux.c.i

app/CMakeFiles/find_the_dot.dir/src/sharedMem-Linux.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/find_the_dot.dir/src/sharedMem-Linux.c.s"
	cd /home/aidle/cmpt433/work/assignment4-1/build/app && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/aidle/cmpt433/work/assignment4-1/app/src/sharedMem-Linux.c -o CMakeFiles/find_the_dot.dir/src/sharedMem-Linux.c.s

# Object files for target find_the_dot
find_the_dot_OBJECTS = \
"CMakeFiles/find_the_dot.dir/src/main.c.o" \
"CMakeFiles/find_the_dot.dir/src/sharedMem-Linux.c.o"

# External object files for target find_the_dot
find_the_dot_EXTERNAL_OBJECTS =

app/find_the_dot: app/CMakeFiles/find_the_dot.dir/src/main.c.o
app/find_the_dot: app/CMakeFiles/find_the_dot.dir/src/sharedMem-Linux.c.o
app/find_the_dot: app/CMakeFiles/find_the_dot.dir/build.make
app/find_the_dot: hal/libhal.a
app/find_the_dot: app/CMakeFiles/find_the_dot.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/aidle/cmpt433/work/assignment4-1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable find_the_dot"
	cd /home/aidle/cmpt433/work/assignment4-1/build/app && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/find_the_dot.dir/link.txt --verbose=$(VERBOSE)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Copying ARM executable to public NFS directory"
	cd /home/aidle/cmpt433/work/assignment4-1/build/app && /usr/bin/cmake -E copy /home/aidle/cmpt433/work/assignment4-1/build/app/find_the_dot ~/cmpt433/public/myApps/find_the_dot

# Rule to build all files generated by this target.
app/CMakeFiles/find_the_dot.dir/build: app/find_the_dot

.PHONY : app/CMakeFiles/find_the_dot.dir/build

app/CMakeFiles/find_the_dot.dir/clean:
	cd /home/aidle/cmpt433/work/assignment4-1/build/app && $(CMAKE_COMMAND) -P CMakeFiles/find_the_dot.dir/cmake_clean.cmake
.PHONY : app/CMakeFiles/find_the_dot.dir/clean

app/CMakeFiles/find_the_dot.dir/depend:
	cd /home/aidle/cmpt433/work/assignment4-1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aidle/cmpt433/work/assignment4-1 /home/aidle/cmpt433/work/assignment4-1/app /home/aidle/cmpt433/work/assignment4-1/build /home/aidle/cmpt433/work/assignment4-1/build/app /home/aidle/cmpt433/work/assignment4-1/build/app/CMakeFiles/find_the_dot.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : app/CMakeFiles/find_the_dot.dir/depend

