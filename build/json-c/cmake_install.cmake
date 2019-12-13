# Install script for directory: /home/user/Public/anteniti_client_server/json-c

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/user/Public/anteniti_client_server/build/json-c/libjson-c.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/json-c/json-c-targets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/json-c/json-c-targets.cmake"
         "/home/user/Public/anteniti_client_server/build/json-c/CMakeFiles/Export/lib/cmake/json-c/json-c-targets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/json-c/json-c-targets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/json-c/json-c-targets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/json-c" TYPE FILE FILES "/home/user/Public/anteniti_client_server/build/json-c/CMakeFiles/Export/lib/cmake/json-c/json-c-targets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/json-c" TYPE FILE FILES "/home/user/Public/anteniti_client_server/build/json-c/CMakeFiles/Export/lib/cmake/json-c/json-c-targets-noconfig.cmake")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/json-c" TYPE FILE FILES "/home/user/Public/anteniti_client_server/build/json-c/json-c-config.cmake")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/pkgconfig/json-c.pc")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib/pkgconfig" TYPE FILE FILES "/home/user/Public/anteniti_client_server/build/json-c/json-c.pc")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/json-c/config.h;/usr/local/include/json-c/json_config.h;/usr/local/include/json-c/json.h;/usr/local/include/json-c/arraylist.h;/usr/local/include/json-c/debug.h;/usr/local/include/json-c/json_c_version.h;/usr/local/include/json-c/json_inttypes.h;/usr/local/include/json-c/json_object.h;/usr/local/include/json-c/json_object_iterator.h;/usr/local/include/json-c/json_pointer.h;/usr/local/include/json-c/json_tokener.h;/usr/local/include/json-c/json_util.h;/usr/local/include/json-c/linkhash.h;/usr/local/include/json-c/printbuf.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/include/json-c" TYPE FILE FILES
    "/home/user/Public/anteniti_client_server/build/json-c/config.h"
    "/home/user/Public/anteniti_client_server/build/json-c/json_config.h"
    "/home/user/Public/anteniti_client_server/json-c/json.h"
    "/home/user/Public/anteniti_client_server/json-c/arraylist.h"
    "/home/user/Public/anteniti_client_server/json-c/debug.h"
    "/home/user/Public/anteniti_client_server/json-c/json_c_version.h"
    "/home/user/Public/anteniti_client_server/json-c/json_inttypes.h"
    "/home/user/Public/anteniti_client_server/json-c/json_object.h"
    "/home/user/Public/anteniti_client_server/json-c/json_object_iterator.h"
    "/home/user/Public/anteniti_client_server/json-c/json_pointer.h"
    "/home/user/Public/anteniti_client_server/json-c/json_tokener.h"
    "/home/user/Public/anteniti_client_server/json-c/json_util.h"
    "/home/user/Public/anteniti_client_server/json-c/linkhash.h"
    "/home/user/Public/anteniti_client_server/json-c/printbuf.h"
    )
endif()

