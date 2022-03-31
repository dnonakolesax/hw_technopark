cmake_minimum_required(VERSION 3.0.0)
project(hw2)

add_subdirectory(k_means_classic)

add_subdirectory(point)

add_executable(${PROJECT_NAME} main.c)

set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-lm")

target_include_directories(${PROJECT_NAME} PUBLIC ${KMEANS_INCLUDE_DIRS})
 
target_link_libraries(${PROJECT_NAME} PRIVATE ${KMEANS_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC ${POINT_INCLUDE_DIRS})
 
target_link_libraries(${PROJECT_NAME} PRIVATE ${POINT_LIBRARIES})

add_subdirectory(test)