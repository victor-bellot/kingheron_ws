cmake_minimum_required(VERSION 3.0.2)
project(heron_mission)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(
    catkin REQUIRED COMPONENTS
    roscpp
    tf
    nav_msgs
    geometry_msgs
    sensor_msgs
    gazebo_msgs
    heron_msgs
    heron_srvs
    cloudy  # for the ObstacleCloud class
)

catkin_package(
    INCLUDE_DIRS include
    CATKIN_DEPENDS roscpp cloudy
)

include_directories(
    INCLUDE_DIRECTORIES include
    ${catkin_INCLUDE_DIRS}
)

# Declare a cpp executable & rename it
add_executable(${PROJECT_NAME}_built src/planner.cpp)

# Rename the node : planner
set_target_properties(
    ${PROJECT_NAME}_built
    PROPERTIES OUTPUT_NAME planner
    PREFIX ""
)

# Specify libraries to link a library or executable target against
target_link_libraries(${PROJECT_NAME}_built ${catkin_LIBRARIES})

# Mark executable Python scripts for installation
catkin_install_python(
    PROGRAMS
    scripts/model_manager.py
    DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

# Mark executables and/or libraries for installation
install(
    TARGETS ${PROJECT_NAME}_built
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(
    DIRECTORY config launch
    DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
