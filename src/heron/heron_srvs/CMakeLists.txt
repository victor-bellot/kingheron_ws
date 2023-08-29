cmake_minimum_required(VERSION 3.0.2)
project(heron_srvs)

find_package(
    catkin REQUIRED COMPONENTS
    std_msgs
    geometry_msgs
    message_generation
)

# Generate services in the 'srv' folder
add_service_files(
    FILES
    GpsMission.srv
    AskModel.srv
    DwaMission.srv
)

generate_messages(
    DEPENDENCIES
    std_msgs
    geometry_msgs
)

catkin_package(CATKIN_DEPENDS std_msgs message_runtime)
