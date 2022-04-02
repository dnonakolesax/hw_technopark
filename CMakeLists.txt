cmake_minimum_required(VERSION 3.0.0)
project(hw2)

if (${ASSYNC})
    add_subdirectory(k_means_multiprocess)
elseif (NOT ${ASSYNC})
    add_subdirectory(k_means_classic)    
endif()

set (CMAKE_C_STANDARD 11)

add_subdirectory(point)
add_subdirectory(kmeans_common)

add_executable(${PROJECT_NAME} main.c)

set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

target_link_libraries(${PROJECT_NAME} PUBLIC m)

target_include_directories(${PROJECT_NAME} PUBLIC ${KMEANS_INCLUDE_DIRS})
 
target_link_libraries(${PROJECT_NAME} PRIVATE ${KMEANS_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC ${KMEANSCOMMON_INCLUDE_DIRS})
 
target_link_libraries(${PROJECT_NAME} PRIVATE ${KMEANSCOMMON_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC ${POINT_INCLUDE_DIRS})
 
target_link_libraries(${PROJECT_NAME} PRIVATE ${POINT_LIBRARIES})

if (${TEST})
    add_subdirectory(test)
endif ()