cmake_minimum_required(VERSION 3.15)
project(joycmd)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()
cmake_policy(SET CMP0074 NEW)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(FetchContent)



include_directories(
  include
  ${rclcpp_INCLUDE_DIRS}
  ${std_msgs_INCLUDE_DIRS}
)

FetchContent_Declare(
  concord
  GIT_REPOSITORY https://github.com/bresilla/concord.git
  GIT_TAG        main
)
FetchContent_MakeAvailable(concord)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

set(dependencies_ros
  rclcpp
  std_msgs
  std_srvs
  geometry_msgs
  sensor_msgs
)


set(dependencies_sys
  concord
)

set(executables
  src/joy_to_cmd_vel.cpp
)

set(exec_names)

foreach(src_file IN LISTS executables)
  get_filename_component(exec_name "${src_file}" NAME_WE)
  add_executable(${exec_name} "${src_file}")
  ament_target_dependencies(${exec_name} ${dependencies_ros})
  target_link_libraries(${exec_name} ${dependencies_sys})
  list(APPEND exec_names ${exec_name})
endforeach()

install(TARGETS
  ${exec_names}
  DESTINATION lib/${PROJECT_NAME}
)


ament_package()
