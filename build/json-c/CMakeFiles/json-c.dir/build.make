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
CMAKE_SOURCE_DIR = /home/user/Public/anteniti_client_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/Public/anteniti_client_server/build

# Include any dependencies generated for this target.
include json-c/CMakeFiles/json-c.dir/depend.make

# Include the progress variables for this target.
include json-c/CMakeFiles/json-c.dir/progress.make

# Include the compile flags for this target's objects.
include json-c/CMakeFiles/json-c.dir/flags.make

json-c/CMakeFiles/json-c.dir/arraylist.c.o: json-c/CMakeFiles/json-c.dir/flags.make
json-c/CMakeFiles/json-c.dir/arraylist.c.o: ../json-c/arraylist.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Public/anteniti_client_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object json-c/CMakeFiles/json-c.dir/arraylist.c.o"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/json-c.dir/arraylist.c.o   -c /home/user/Public/anteniti_client_server/json-c/arraylist.c

json-c/CMakeFiles/json-c.dir/arraylist.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/json-c.dir/arraylist.c.i"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/Public/anteniti_client_server/json-c/arraylist.c > CMakeFiles/json-c.dir/arraylist.c.i

json-c/CMakeFiles/json-c.dir/arraylist.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/json-c.dir/arraylist.c.s"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/Public/anteniti_client_server/json-c/arraylist.c -o CMakeFiles/json-c.dir/arraylist.c.s

json-c/CMakeFiles/json-c.dir/arraylist.c.o.requires:

.PHONY : json-c/CMakeFiles/json-c.dir/arraylist.c.o.requires

json-c/CMakeFiles/json-c.dir/arraylist.c.o.provides: json-c/CMakeFiles/json-c.dir/arraylist.c.o.requires
	$(MAKE) -f json-c/CMakeFiles/json-c.dir/build.make json-c/CMakeFiles/json-c.dir/arraylist.c.o.provides.build
.PHONY : json-c/CMakeFiles/json-c.dir/arraylist.c.o.provides

json-c/CMakeFiles/json-c.dir/arraylist.c.o.provides.build: json-c/CMakeFiles/json-c.dir/arraylist.c.o


json-c/CMakeFiles/json-c.dir/debug.c.o: json-c/CMakeFiles/json-c.dir/flags.make
json-c/CMakeFiles/json-c.dir/debug.c.o: ../json-c/debug.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Public/anteniti_client_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object json-c/CMakeFiles/json-c.dir/debug.c.o"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/json-c.dir/debug.c.o   -c /home/user/Public/anteniti_client_server/json-c/debug.c

json-c/CMakeFiles/json-c.dir/debug.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/json-c.dir/debug.c.i"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/Public/anteniti_client_server/json-c/debug.c > CMakeFiles/json-c.dir/debug.c.i

json-c/CMakeFiles/json-c.dir/debug.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/json-c.dir/debug.c.s"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/Public/anteniti_client_server/json-c/debug.c -o CMakeFiles/json-c.dir/debug.c.s

json-c/CMakeFiles/json-c.dir/debug.c.o.requires:

.PHONY : json-c/CMakeFiles/json-c.dir/debug.c.o.requires

json-c/CMakeFiles/json-c.dir/debug.c.o.provides: json-c/CMakeFiles/json-c.dir/debug.c.o.requires
	$(MAKE) -f json-c/CMakeFiles/json-c.dir/build.make json-c/CMakeFiles/json-c.dir/debug.c.o.provides.build
.PHONY : json-c/CMakeFiles/json-c.dir/debug.c.o.provides

json-c/CMakeFiles/json-c.dir/debug.c.o.provides.build: json-c/CMakeFiles/json-c.dir/debug.c.o


json-c/CMakeFiles/json-c.dir/json_c_version.c.o: json-c/CMakeFiles/json-c.dir/flags.make
json-c/CMakeFiles/json-c.dir/json_c_version.c.o: ../json-c/json_c_version.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Public/anteniti_client_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object json-c/CMakeFiles/json-c.dir/json_c_version.c.o"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/json-c.dir/json_c_version.c.o   -c /home/user/Public/anteniti_client_server/json-c/json_c_version.c

json-c/CMakeFiles/json-c.dir/json_c_version.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/json-c.dir/json_c_version.c.i"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/Public/anteniti_client_server/json-c/json_c_version.c > CMakeFiles/json-c.dir/json_c_version.c.i

json-c/CMakeFiles/json-c.dir/json_c_version.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/json-c.dir/json_c_version.c.s"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/Public/anteniti_client_server/json-c/json_c_version.c -o CMakeFiles/json-c.dir/json_c_version.c.s

json-c/CMakeFiles/json-c.dir/json_c_version.c.o.requires:

.PHONY : json-c/CMakeFiles/json-c.dir/json_c_version.c.o.requires

json-c/CMakeFiles/json-c.dir/json_c_version.c.o.provides: json-c/CMakeFiles/json-c.dir/json_c_version.c.o.requires
	$(MAKE) -f json-c/CMakeFiles/json-c.dir/build.make json-c/CMakeFiles/json-c.dir/json_c_version.c.o.provides.build
.PHONY : json-c/CMakeFiles/json-c.dir/json_c_version.c.o.provides

json-c/CMakeFiles/json-c.dir/json_c_version.c.o.provides.build: json-c/CMakeFiles/json-c.dir/json_c_version.c.o


json-c/CMakeFiles/json-c.dir/json_object.c.o: json-c/CMakeFiles/json-c.dir/flags.make
json-c/CMakeFiles/json-c.dir/json_object.c.o: ../json-c/json_object.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Public/anteniti_client_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object json-c/CMakeFiles/json-c.dir/json_object.c.o"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/json-c.dir/json_object.c.o   -c /home/user/Public/anteniti_client_server/json-c/json_object.c

json-c/CMakeFiles/json-c.dir/json_object.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/json-c.dir/json_object.c.i"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/Public/anteniti_client_server/json-c/json_object.c > CMakeFiles/json-c.dir/json_object.c.i

json-c/CMakeFiles/json-c.dir/json_object.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/json-c.dir/json_object.c.s"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/Public/anteniti_client_server/json-c/json_object.c -o CMakeFiles/json-c.dir/json_object.c.s

json-c/CMakeFiles/json-c.dir/json_object.c.o.requires:

.PHONY : json-c/CMakeFiles/json-c.dir/json_object.c.o.requires

json-c/CMakeFiles/json-c.dir/json_object.c.o.provides: json-c/CMakeFiles/json-c.dir/json_object.c.o.requires
	$(MAKE) -f json-c/CMakeFiles/json-c.dir/build.make json-c/CMakeFiles/json-c.dir/json_object.c.o.provides.build
.PHONY : json-c/CMakeFiles/json-c.dir/json_object.c.o.provides

json-c/CMakeFiles/json-c.dir/json_object.c.o.provides.build: json-c/CMakeFiles/json-c.dir/json_object.c.o


json-c/CMakeFiles/json-c.dir/json_object_iterator.c.o: json-c/CMakeFiles/json-c.dir/flags.make
json-c/CMakeFiles/json-c.dir/json_object_iterator.c.o: ../json-c/json_object_iterator.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Public/anteniti_client_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object json-c/CMakeFiles/json-c.dir/json_object_iterator.c.o"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/json-c.dir/json_object_iterator.c.o   -c /home/user/Public/anteniti_client_server/json-c/json_object_iterator.c

json-c/CMakeFiles/json-c.dir/json_object_iterator.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/json-c.dir/json_object_iterator.c.i"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/Public/anteniti_client_server/json-c/json_object_iterator.c > CMakeFiles/json-c.dir/json_object_iterator.c.i

json-c/CMakeFiles/json-c.dir/json_object_iterator.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/json-c.dir/json_object_iterator.c.s"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/Public/anteniti_client_server/json-c/json_object_iterator.c -o CMakeFiles/json-c.dir/json_object_iterator.c.s

json-c/CMakeFiles/json-c.dir/json_object_iterator.c.o.requires:

.PHONY : json-c/CMakeFiles/json-c.dir/json_object_iterator.c.o.requires

json-c/CMakeFiles/json-c.dir/json_object_iterator.c.o.provides: json-c/CMakeFiles/json-c.dir/json_object_iterator.c.o.requires
	$(MAKE) -f json-c/CMakeFiles/json-c.dir/build.make json-c/CMakeFiles/json-c.dir/json_object_iterator.c.o.provides.build
.PHONY : json-c/CMakeFiles/json-c.dir/json_object_iterator.c.o.provides

json-c/CMakeFiles/json-c.dir/json_object_iterator.c.o.provides.build: json-c/CMakeFiles/json-c.dir/json_object_iterator.c.o


json-c/CMakeFiles/json-c.dir/json_pointer.c.o: json-c/CMakeFiles/json-c.dir/flags.make
json-c/CMakeFiles/json-c.dir/json_pointer.c.o: ../json-c/json_pointer.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Public/anteniti_client_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object json-c/CMakeFiles/json-c.dir/json_pointer.c.o"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/json-c.dir/json_pointer.c.o   -c /home/user/Public/anteniti_client_server/json-c/json_pointer.c

json-c/CMakeFiles/json-c.dir/json_pointer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/json-c.dir/json_pointer.c.i"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/Public/anteniti_client_server/json-c/json_pointer.c > CMakeFiles/json-c.dir/json_pointer.c.i

json-c/CMakeFiles/json-c.dir/json_pointer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/json-c.dir/json_pointer.c.s"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/Public/anteniti_client_server/json-c/json_pointer.c -o CMakeFiles/json-c.dir/json_pointer.c.s

json-c/CMakeFiles/json-c.dir/json_pointer.c.o.requires:

.PHONY : json-c/CMakeFiles/json-c.dir/json_pointer.c.o.requires

json-c/CMakeFiles/json-c.dir/json_pointer.c.o.provides: json-c/CMakeFiles/json-c.dir/json_pointer.c.o.requires
	$(MAKE) -f json-c/CMakeFiles/json-c.dir/build.make json-c/CMakeFiles/json-c.dir/json_pointer.c.o.provides.build
.PHONY : json-c/CMakeFiles/json-c.dir/json_pointer.c.o.provides

json-c/CMakeFiles/json-c.dir/json_pointer.c.o.provides.build: json-c/CMakeFiles/json-c.dir/json_pointer.c.o


json-c/CMakeFiles/json-c.dir/json_tokener.c.o: json-c/CMakeFiles/json-c.dir/flags.make
json-c/CMakeFiles/json-c.dir/json_tokener.c.o: ../json-c/json_tokener.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Public/anteniti_client_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object json-c/CMakeFiles/json-c.dir/json_tokener.c.o"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/json-c.dir/json_tokener.c.o   -c /home/user/Public/anteniti_client_server/json-c/json_tokener.c

json-c/CMakeFiles/json-c.dir/json_tokener.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/json-c.dir/json_tokener.c.i"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/Public/anteniti_client_server/json-c/json_tokener.c > CMakeFiles/json-c.dir/json_tokener.c.i

json-c/CMakeFiles/json-c.dir/json_tokener.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/json-c.dir/json_tokener.c.s"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/Public/anteniti_client_server/json-c/json_tokener.c -o CMakeFiles/json-c.dir/json_tokener.c.s

json-c/CMakeFiles/json-c.dir/json_tokener.c.o.requires:

.PHONY : json-c/CMakeFiles/json-c.dir/json_tokener.c.o.requires

json-c/CMakeFiles/json-c.dir/json_tokener.c.o.provides: json-c/CMakeFiles/json-c.dir/json_tokener.c.o.requires
	$(MAKE) -f json-c/CMakeFiles/json-c.dir/build.make json-c/CMakeFiles/json-c.dir/json_tokener.c.o.provides.build
.PHONY : json-c/CMakeFiles/json-c.dir/json_tokener.c.o.provides

json-c/CMakeFiles/json-c.dir/json_tokener.c.o.provides.build: json-c/CMakeFiles/json-c.dir/json_tokener.c.o


json-c/CMakeFiles/json-c.dir/json_util.c.o: json-c/CMakeFiles/json-c.dir/flags.make
json-c/CMakeFiles/json-c.dir/json_util.c.o: ../json-c/json_util.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Public/anteniti_client_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object json-c/CMakeFiles/json-c.dir/json_util.c.o"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/json-c.dir/json_util.c.o   -c /home/user/Public/anteniti_client_server/json-c/json_util.c

json-c/CMakeFiles/json-c.dir/json_util.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/json-c.dir/json_util.c.i"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/Public/anteniti_client_server/json-c/json_util.c > CMakeFiles/json-c.dir/json_util.c.i

json-c/CMakeFiles/json-c.dir/json_util.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/json-c.dir/json_util.c.s"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/Public/anteniti_client_server/json-c/json_util.c -o CMakeFiles/json-c.dir/json_util.c.s

json-c/CMakeFiles/json-c.dir/json_util.c.o.requires:

.PHONY : json-c/CMakeFiles/json-c.dir/json_util.c.o.requires

json-c/CMakeFiles/json-c.dir/json_util.c.o.provides: json-c/CMakeFiles/json-c.dir/json_util.c.o.requires
	$(MAKE) -f json-c/CMakeFiles/json-c.dir/build.make json-c/CMakeFiles/json-c.dir/json_util.c.o.provides.build
.PHONY : json-c/CMakeFiles/json-c.dir/json_util.c.o.provides

json-c/CMakeFiles/json-c.dir/json_util.c.o.provides.build: json-c/CMakeFiles/json-c.dir/json_util.c.o


json-c/CMakeFiles/json-c.dir/json_visit.c.o: json-c/CMakeFiles/json-c.dir/flags.make
json-c/CMakeFiles/json-c.dir/json_visit.c.o: ../json-c/json_visit.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Public/anteniti_client_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object json-c/CMakeFiles/json-c.dir/json_visit.c.o"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/json-c.dir/json_visit.c.o   -c /home/user/Public/anteniti_client_server/json-c/json_visit.c

json-c/CMakeFiles/json-c.dir/json_visit.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/json-c.dir/json_visit.c.i"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/Public/anteniti_client_server/json-c/json_visit.c > CMakeFiles/json-c.dir/json_visit.c.i

json-c/CMakeFiles/json-c.dir/json_visit.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/json-c.dir/json_visit.c.s"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/Public/anteniti_client_server/json-c/json_visit.c -o CMakeFiles/json-c.dir/json_visit.c.s

json-c/CMakeFiles/json-c.dir/json_visit.c.o.requires:

.PHONY : json-c/CMakeFiles/json-c.dir/json_visit.c.o.requires

json-c/CMakeFiles/json-c.dir/json_visit.c.o.provides: json-c/CMakeFiles/json-c.dir/json_visit.c.o.requires
	$(MAKE) -f json-c/CMakeFiles/json-c.dir/build.make json-c/CMakeFiles/json-c.dir/json_visit.c.o.provides.build
.PHONY : json-c/CMakeFiles/json-c.dir/json_visit.c.o.provides

json-c/CMakeFiles/json-c.dir/json_visit.c.o.provides.build: json-c/CMakeFiles/json-c.dir/json_visit.c.o


json-c/CMakeFiles/json-c.dir/linkhash.c.o: json-c/CMakeFiles/json-c.dir/flags.make
json-c/CMakeFiles/json-c.dir/linkhash.c.o: ../json-c/linkhash.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Public/anteniti_client_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object json-c/CMakeFiles/json-c.dir/linkhash.c.o"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/json-c.dir/linkhash.c.o   -c /home/user/Public/anteniti_client_server/json-c/linkhash.c

json-c/CMakeFiles/json-c.dir/linkhash.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/json-c.dir/linkhash.c.i"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/Public/anteniti_client_server/json-c/linkhash.c > CMakeFiles/json-c.dir/linkhash.c.i

json-c/CMakeFiles/json-c.dir/linkhash.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/json-c.dir/linkhash.c.s"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/Public/anteniti_client_server/json-c/linkhash.c -o CMakeFiles/json-c.dir/linkhash.c.s

json-c/CMakeFiles/json-c.dir/linkhash.c.o.requires:

.PHONY : json-c/CMakeFiles/json-c.dir/linkhash.c.o.requires

json-c/CMakeFiles/json-c.dir/linkhash.c.o.provides: json-c/CMakeFiles/json-c.dir/linkhash.c.o.requires
	$(MAKE) -f json-c/CMakeFiles/json-c.dir/build.make json-c/CMakeFiles/json-c.dir/linkhash.c.o.provides.build
.PHONY : json-c/CMakeFiles/json-c.dir/linkhash.c.o.provides

json-c/CMakeFiles/json-c.dir/linkhash.c.o.provides.build: json-c/CMakeFiles/json-c.dir/linkhash.c.o


json-c/CMakeFiles/json-c.dir/printbuf.c.o: json-c/CMakeFiles/json-c.dir/flags.make
json-c/CMakeFiles/json-c.dir/printbuf.c.o: ../json-c/printbuf.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Public/anteniti_client_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object json-c/CMakeFiles/json-c.dir/printbuf.c.o"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/json-c.dir/printbuf.c.o   -c /home/user/Public/anteniti_client_server/json-c/printbuf.c

json-c/CMakeFiles/json-c.dir/printbuf.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/json-c.dir/printbuf.c.i"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/Public/anteniti_client_server/json-c/printbuf.c > CMakeFiles/json-c.dir/printbuf.c.i

json-c/CMakeFiles/json-c.dir/printbuf.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/json-c.dir/printbuf.c.s"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/Public/anteniti_client_server/json-c/printbuf.c -o CMakeFiles/json-c.dir/printbuf.c.s

json-c/CMakeFiles/json-c.dir/printbuf.c.o.requires:

.PHONY : json-c/CMakeFiles/json-c.dir/printbuf.c.o.requires

json-c/CMakeFiles/json-c.dir/printbuf.c.o.provides: json-c/CMakeFiles/json-c.dir/printbuf.c.o.requires
	$(MAKE) -f json-c/CMakeFiles/json-c.dir/build.make json-c/CMakeFiles/json-c.dir/printbuf.c.o.provides.build
.PHONY : json-c/CMakeFiles/json-c.dir/printbuf.c.o.provides

json-c/CMakeFiles/json-c.dir/printbuf.c.o.provides.build: json-c/CMakeFiles/json-c.dir/printbuf.c.o


json-c/CMakeFiles/json-c.dir/random_seed.c.o: json-c/CMakeFiles/json-c.dir/flags.make
json-c/CMakeFiles/json-c.dir/random_seed.c.o: ../json-c/random_seed.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Public/anteniti_client_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building C object json-c/CMakeFiles/json-c.dir/random_seed.c.o"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/json-c.dir/random_seed.c.o   -c /home/user/Public/anteniti_client_server/json-c/random_seed.c

json-c/CMakeFiles/json-c.dir/random_seed.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/json-c.dir/random_seed.c.i"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/Public/anteniti_client_server/json-c/random_seed.c > CMakeFiles/json-c.dir/random_seed.c.i

json-c/CMakeFiles/json-c.dir/random_seed.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/json-c.dir/random_seed.c.s"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/Public/anteniti_client_server/json-c/random_seed.c -o CMakeFiles/json-c.dir/random_seed.c.s

json-c/CMakeFiles/json-c.dir/random_seed.c.o.requires:

.PHONY : json-c/CMakeFiles/json-c.dir/random_seed.c.o.requires

json-c/CMakeFiles/json-c.dir/random_seed.c.o.provides: json-c/CMakeFiles/json-c.dir/random_seed.c.o.requires
	$(MAKE) -f json-c/CMakeFiles/json-c.dir/build.make json-c/CMakeFiles/json-c.dir/random_seed.c.o.provides.build
.PHONY : json-c/CMakeFiles/json-c.dir/random_seed.c.o.provides

json-c/CMakeFiles/json-c.dir/random_seed.c.o.provides.build: json-c/CMakeFiles/json-c.dir/random_seed.c.o


json-c/CMakeFiles/json-c.dir/strerror_override.c.o: json-c/CMakeFiles/json-c.dir/flags.make
json-c/CMakeFiles/json-c.dir/strerror_override.c.o: ../json-c/strerror_override.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Public/anteniti_client_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building C object json-c/CMakeFiles/json-c.dir/strerror_override.c.o"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/json-c.dir/strerror_override.c.o   -c /home/user/Public/anteniti_client_server/json-c/strerror_override.c

json-c/CMakeFiles/json-c.dir/strerror_override.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/json-c.dir/strerror_override.c.i"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/Public/anteniti_client_server/json-c/strerror_override.c > CMakeFiles/json-c.dir/strerror_override.c.i

json-c/CMakeFiles/json-c.dir/strerror_override.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/json-c.dir/strerror_override.c.s"
	cd /home/user/Public/anteniti_client_server/build/json-c && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/Public/anteniti_client_server/json-c/strerror_override.c -o CMakeFiles/json-c.dir/strerror_override.c.s

json-c/CMakeFiles/json-c.dir/strerror_override.c.o.requires:

.PHONY : json-c/CMakeFiles/json-c.dir/strerror_override.c.o.requires

json-c/CMakeFiles/json-c.dir/strerror_override.c.o.provides: json-c/CMakeFiles/json-c.dir/strerror_override.c.o.requires
	$(MAKE) -f json-c/CMakeFiles/json-c.dir/build.make json-c/CMakeFiles/json-c.dir/strerror_override.c.o.provides.build
.PHONY : json-c/CMakeFiles/json-c.dir/strerror_override.c.o.provides

json-c/CMakeFiles/json-c.dir/strerror_override.c.o.provides.build: json-c/CMakeFiles/json-c.dir/strerror_override.c.o


# Object files for target json-c
json__c_OBJECTS = \
"CMakeFiles/json-c.dir/arraylist.c.o" \
"CMakeFiles/json-c.dir/debug.c.o" \
"CMakeFiles/json-c.dir/json_c_version.c.o" \
"CMakeFiles/json-c.dir/json_object.c.o" \
"CMakeFiles/json-c.dir/json_object_iterator.c.o" \
"CMakeFiles/json-c.dir/json_pointer.c.o" \
"CMakeFiles/json-c.dir/json_tokener.c.o" \
"CMakeFiles/json-c.dir/json_util.c.o" \
"CMakeFiles/json-c.dir/json_visit.c.o" \
"CMakeFiles/json-c.dir/linkhash.c.o" \
"CMakeFiles/json-c.dir/printbuf.c.o" \
"CMakeFiles/json-c.dir/random_seed.c.o" \
"CMakeFiles/json-c.dir/strerror_override.c.o"

# External object files for target json-c
json__c_EXTERNAL_OBJECTS =

json-c/libjson-c.a: json-c/CMakeFiles/json-c.dir/arraylist.c.o
json-c/libjson-c.a: json-c/CMakeFiles/json-c.dir/debug.c.o
json-c/libjson-c.a: json-c/CMakeFiles/json-c.dir/json_c_version.c.o
json-c/libjson-c.a: json-c/CMakeFiles/json-c.dir/json_object.c.o
json-c/libjson-c.a: json-c/CMakeFiles/json-c.dir/json_object_iterator.c.o
json-c/libjson-c.a: json-c/CMakeFiles/json-c.dir/json_pointer.c.o
json-c/libjson-c.a: json-c/CMakeFiles/json-c.dir/json_tokener.c.o
json-c/libjson-c.a: json-c/CMakeFiles/json-c.dir/json_util.c.o
json-c/libjson-c.a: json-c/CMakeFiles/json-c.dir/json_visit.c.o
json-c/libjson-c.a: json-c/CMakeFiles/json-c.dir/linkhash.c.o
json-c/libjson-c.a: json-c/CMakeFiles/json-c.dir/printbuf.c.o
json-c/libjson-c.a: json-c/CMakeFiles/json-c.dir/random_seed.c.o
json-c/libjson-c.a: json-c/CMakeFiles/json-c.dir/strerror_override.c.o
json-c/libjson-c.a: json-c/CMakeFiles/json-c.dir/build.make
json-c/libjson-c.a: json-c/CMakeFiles/json-c.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/user/Public/anteniti_client_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Linking C static library libjson-c.a"
	cd /home/user/Public/anteniti_client_server/build/json-c && $(CMAKE_COMMAND) -P CMakeFiles/json-c.dir/cmake_clean_target.cmake
	cd /home/user/Public/anteniti_client_server/build/json-c && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/json-c.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
json-c/CMakeFiles/json-c.dir/build: json-c/libjson-c.a

.PHONY : json-c/CMakeFiles/json-c.dir/build

json-c/CMakeFiles/json-c.dir/requires: json-c/CMakeFiles/json-c.dir/arraylist.c.o.requires
json-c/CMakeFiles/json-c.dir/requires: json-c/CMakeFiles/json-c.dir/debug.c.o.requires
json-c/CMakeFiles/json-c.dir/requires: json-c/CMakeFiles/json-c.dir/json_c_version.c.o.requires
json-c/CMakeFiles/json-c.dir/requires: json-c/CMakeFiles/json-c.dir/json_object.c.o.requires
json-c/CMakeFiles/json-c.dir/requires: json-c/CMakeFiles/json-c.dir/json_object_iterator.c.o.requires
json-c/CMakeFiles/json-c.dir/requires: json-c/CMakeFiles/json-c.dir/json_pointer.c.o.requires
json-c/CMakeFiles/json-c.dir/requires: json-c/CMakeFiles/json-c.dir/json_tokener.c.o.requires
json-c/CMakeFiles/json-c.dir/requires: json-c/CMakeFiles/json-c.dir/json_util.c.o.requires
json-c/CMakeFiles/json-c.dir/requires: json-c/CMakeFiles/json-c.dir/json_visit.c.o.requires
json-c/CMakeFiles/json-c.dir/requires: json-c/CMakeFiles/json-c.dir/linkhash.c.o.requires
json-c/CMakeFiles/json-c.dir/requires: json-c/CMakeFiles/json-c.dir/printbuf.c.o.requires
json-c/CMakeFiles/json-c.dir/requires: json-c/CMakeFiles/json-c.dir/random_seed.c.o.requires
json-c/CMakeFiles/json-c.dir/requires: json-c/CMakeFiles/json-c.dir/strerror_override.c.o.requires

.PHONY : json-c/CMakeFiles/json-c.dir/requires

json-c/CMakeFiles/json-c.dir/clean:
	cd /home/user/Public/anteniti_client_server/build/json-c && $(CMAKE_COMMAND) -P CMakeFiles/json-c.dir/cmake_clean.cmake
.PHONY : json-c/CMakeFiles/json-c.dir/clean

json-c/CMakeFiles/json-c.dir/depend:
	cd /home/user/Public/anteniti_client_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/Public/anteniti_client_server /home/user/Public/anteniti_client_server/json-c /home/user/Public/anteniti_client_server/build /home/user/Public/anteniti_client_server/build/json-c /home/user/Public/anteniti_client_server/build/json-c/CMakeFiles/json-c.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : json-c/CMakeFiles/json-c.dir/depend

