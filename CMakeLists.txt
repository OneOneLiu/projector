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
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES basler_control
#  CATKIN_DEPENDS rospy roscpp
#  DEPENDS system_lib
)


## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

# Find projector source, main and header files
aux_source_directory(${PROJECT_SOURCE_DIR}/src/  PR_SOURCE_FILES)
set(PR_MAIN_FILE ${PROJECT_SOURCE_DIR}/src/nodes/test_projector.cpp)
file(GLOB PR_HEADER_FILES ${PROJECT_SOURCE_DIR}/include/projector/*.h)
message("Source file list: ${PR_SOURCE_FILES}")
message("Header file list: ${PR_HEADER_FILES}")
set(PROJECTOR_LINK_LIBS /usr/lib/x86_64-linux-gnu/libudev.so)

# Compile the projector library from source and header files
add_library(Projector SHARED ${PR_SOURCE_FILES} ${PR_HEADER_FILES})
target_include_directories(Projector PUBLIC ${PROJECT_SOURCE_DIR}/include/projector)
target_link_libraries(Projector PUBLIC ${PROJECTOR_LINK_LIBS} ${catkin_LIBRARIES})

# Compile the executable file from main.cpp and link it to the pre-compiled library
add_executable(test_projector ${PR_MAIN_FILE})
target_link_libraries(test_projector PRIVATE Projector)