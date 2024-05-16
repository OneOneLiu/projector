cmake_minimum_required(VERSION 3.0.2)
project(projector)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
)

# Add services
## Generate services in the 'srv' folder
add_service_files(
  FILES
  Projection.srv
)

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS message_runtime std_msgs roscpp rospy
)


## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

# Find projector source, main and header files
aux_source_directory(${PROJECT_SOURCE_DIR}/src/  PR_SOURCE_FILES)
set(PR_TEST_FILE ${PROJECT_SOURCE_DIR}/src/nodes/test_projector.cpp)
set(PR_SERVER_FILE ${PROJECT_SOURCE_DIR}/src/nodes/projector_server.cpp)

file(GLOB PR_HEADER_FILES ${PROJECT_SOURCE_DIR}/include/projector/*.h)
message("Source file list: ${PR_SOURCE_FILES}")
message("Header file list: ${PR_HEADER_FILES}")
set(PROJECTOR_LINK_LIBS /usr/lib/x86_64-linux-gnu/libudev.so)

# Compile the projector library from source and header files
add_library(Projector SHARED ${PR_SOURCE_FILES} ${PR_HEADER_FILES})
target_include_directories(Projector PUBLIC ${PROJECT_SOURCE_DIR}/include/projector)
target_link_libraries(Projector PUBLIC ${PROJECTOR_LINK_LIBS} ${catkin_LIBRARIES})

# Compile the executable file from main.cpp and link it to the pre-compiled library
add_executable(test_projector ${PR_TEST_FILE})
target_link_libraries(test_projector PRIVATE Projector)

add_executable(projector_server ${PR_SERVER_FILE})
target_link_libraries(projector_server PRIVATE Projector)

catkin_install_python(PROGRAMS
  src/nodes/projector_client.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)