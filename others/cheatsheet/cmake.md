```cmake
cmake_minimum_required(VERSION 3.10)
project(CXX_TEST)

#======== Standard
set(CMAKE_CXX_STANDARD 17)
#======== Source Name
set(SRC main.cpp)

#======== Compile Options
set(CMAKE_CXX_FLAGS "")

## set(CMAKE_CXX_FLAGS "-Ofast -mpopcnt -march=native")
## if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" || "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

#======== Ignore.
add_executable(${PROJECT_NAME} ${SRC})

#======== Packages.
find_package(TBB REQUIRED)
if (TBB_FOUND)
    message(STATUS ">>>> TBB found!")
    #    include_directories(${TBB_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${TBB_IMPORTED_TARGETS})
endif ()

find_package(OpenMP)
if(OpenMP_FOUND)
    message(STATUS ">>>> OPENMP found!")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

find_package(OpenCV 4 REQUIRED)
if(OpenCV_FOUND)
    message(STATUS ">>>> OpenCV found!")
    message(STATUS "    version: ${OpenCV_VERSION}")
    message(STATUS "    libraries: ${OpenCV_LIBS}")
    message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
    message(STATUS "    cmake: ${OpenCV_DIR}")
    #    include_directories(${OpenCV_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
endif()

message(STATUS ${CMAKE_PREFIX_PATH})

find_package(PkgConfig REQUIRED)
pkg_search_module(GRPC REQUIRED grpc)
pkg_search_module(GRPCPP REQUIRED grpc++>=1.26.0)

message(STATUS "Final compile options: ${CMAKE_CXX_FLAGS}")

include_directories(taskflow)
```

