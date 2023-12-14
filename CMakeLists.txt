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
    m
)

include_directories(${INCLUDE_DIRS})

add_executable(day-01-p1 source/day-01-p1.c)
target_link_libraries(day-01-p1 ${LIBRARIES})
add_executable(day-01-p2 source/day-01-p2.c)
target_link_libraries(day-01-p2 ${LIBRARIES})

add_executable(day-02-p1 source/day-02-p1.c)
target_link_libraries(day-02-p1 ${LIBRARIES})
add_executable(day-02-p2 source/day-02-p2.c)
target_link_libraries(day-02-p2 ${LIBRARIES})

add_executable(day-03-p1 source/day-03-p1.c)
target_link_libraries(day-03-p1 ${LIBRARIES})
add_executable(day-03-p2 source/day-03-p2.c)
target_link_libraries(day-03-p2 ${LIBRARIES})

add_executable(day-04-p1 source/day-04-p1.c)
target_link_libraries(day-04-p1 ${LIBRARIES})
add_executable(day-04-p2 source/day-04-p2.c)
target_link_libraries(day-04-p2 ${LIBRARIES})

add_executable(day-05-p1 source/day-05-p1.c)
target_link_libraries(day-05-p1 ${LIBRARIES})
add_executable(day-05-p2 source/day-05-p2.c)
target_link_libraries(day-05-p2 ${LIBRARIES})

add_executable(day-06-p1 source/day-06-p1.c)
target_link_libraries(day-06-p1 ${LIBRARIES})
add_executable(day-06-p2 source/day-06-p2.c)
target_link_libraries(day-06-p2 ${LIBRARIES})

add_executable(day-07-p1 source/day-07-p1.c)
target_link_libraries(day-07-p1 ${LIBRARIES})
add_executable(day-07-p2 source/day-07-p2.c)
target_link_libraries(day-07-p2 ${LIBRARIES})

add_executable(day-08-p1 source/day-08-p1.c)
target_link_libraries(day-08-p1 ${LIBRARIES})
add_executable(day-08-p2 source/day-08-p2.c)
target_link_libraries(day-08-p2 ${LIBRARIES})

add_executable(day-09-p1 source/day-09-p1.c)
target_link_libraries(day-09-p1 ${LIBRARIES})
add_executable(day-09-p2 source/day-09-p2.c)
target_link_libraries(day-09-p2 ${LIBRARIES})

add_executable(day-10-p1 source/day-10-p1.c)
target_link_libraries(day-10-p1 ${LIBRARIES})
add_executable(day-10-p2 source/day-10-p2.c)
target_link_libraries(day-10-p2 ${LIBRARIES})