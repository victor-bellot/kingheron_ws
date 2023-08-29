cmake_minimum_required(VERSION 3.0.2)
project(cloudy)

set(CMAKE_CXX_STANDARD 11)

find_package(catkin REQUIRED
    COMPONENTS
        roscpp
        sensor_msgs
        geometry_msgs
        tf
        laser_geometry  # for the LaserProjection class
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS roscpp
)

include_directories(
    include
    ${catkin_INCLUDE_DIRS}
)

add_library(
    ${PROJECT_NAME}
    src/obstacle_cloud.cpp
    src/trajectory_circle.cpp
    src/tools.cpp
)
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES})

install(
    TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
)
install(
    DIRECTORY include/${PROJECT_NAME}/
    DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

# Build the ScanToCloud node
add_executable(scan_to_cloud src/scan_to_cloud.cpp)
target_link_libraries(scan_to_cloud ${catkin_LIBRARIES})
install(TARGETS scan_to_cloud RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
