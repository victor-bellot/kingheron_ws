# Install script for directory: /home/kingheron/kingheron_ws/src/heron_robot/heron_bringup

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/kingheron/kingheron_ws/install")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/kingheron/kingheron_ws/build/heron_robot/heron_bringup/catkin_generated/installspace/heron_bringup.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/heron_bringup/cmake" TYPE FILE FILES
    "/home/kingheron/kingheron_ws/build/heron_robot/heron_bringup/catkin_generated/installspace/heron_bringupConfig.cmake"
    "/home/kingheron/kingheron_ws/build/heron_robot/heron_bringup/catkin_generated/installspace/heron_bringupConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/heron_bringup" TYPE FILE FILES "/home/kingheron/kingheron_ws/src/heron_robot/heron_bringup/package.xml")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/kingheron/kingheron_ws/build/heron_robot/heron_bringup/catkin_generated/safe_execute_install.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/heron_bringup" TYPE DIRECTORY FILES
    "/home/kingheron/kingheron_ws/src/heron_robot/heron_bringup/launch"
    "/home/kingheron/kingheron_ws/src/heron_robot/heron_bringup/config"
    "/home/kingheron/kingheron_ws/src/heron_robot/heron_bringup/udev"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/heron_bringup" TYPE PROGRAM FILES
    "/home/kingheron/kingheron_ws/src/heron_robot/heron_bringup/scripts/calibrate_compass"
    "/home/kingheron/kingheron_ws/src/heron_robot/heron_bringup/scripts/compute_calibration"
    "/home/kingheron/kingheron_ws/src/heron_robot/heron_bringup/scripts/install"
    "/home/kingheron/kingheron_ws/src/heron_robot/heron_bringup/scripts/navsat_rtk_relay"
    "/home/kingheron/kingheron_ws/src/heron_robot/heron_bringup/scripts/netserial"
    "/home/kingheron/kingheron_ws/src/heron_robot/heron_bringup/scripts/setup"
    )
endif()

