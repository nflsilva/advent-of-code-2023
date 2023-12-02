cmake_minimum_required(VERSION 3.26.4)

project(advent-of-code-2023)

# cutil dependency
add_subdirectory(dependencies/cutil)

set(
    INCLUDE_DIRS
    dependencies/cutil/include
)

set(
    LIBRARIES
    cutil
)

include_directories(${INCLUDE_DIRS})

add_executable(day-1-p1 source/day-1-p1.c)
target_link_libraries(day-1-p1 ${LIBRARIES})
add_executable(day-1-p2 source/day-1-p2.c)
target_link_libraries(day-1-p2 ${LIBRARIES})