cmake_minimum_required(VERSION 3.26.4)

project(cutil)

set(CUTIL_SOURCES
  source/hashTable.c
  source/list.c
  source/filesystem.c
  source/math.c
)

include_directories(include)

add_library(cutil STATIC ${CUTIL_SOURCES})

include(CTest)
add_subdirectory(tests)