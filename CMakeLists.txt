cmake_minimum_required(VERSION 3.0.0)
project(hw2)

<<<<<<< Updated upstream
add_subdirectory(k_means_classic)
=======
if (${ASSYNC})
    add_subdirectory(k_means_multiprocess)
elseif (NOT ${ASSYNC})
    add_subdirectory(k_means_classic)    
endif()
>>>>>>> Stashed changes

add_subdirectory(point)

add_executable(${PROJECT_NAME} main.c)

set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
<<<<<<< Updated upstream
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-lm")
=======
>>>>>>> Stashed changes

target_include_directories(${PROJECT_NAME} PUBLIC ${KMEANS_INCLUDE_DIRS})
 
target_link_libraries(${PROJECT_NAME} PRIVATE ${KMEANS_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC ${POINT_INCLUDE_DIRS})
 
target_link_libraries(${PROJECT_NAME} PRIVATE ${POINT_LIBRARIES})

<<<<<<< Updated upstream
add_subdirectory(test)
=======
if (${TEST})
    add_subdirectory(test)
endif ()
>>>>>>> Stashed changes
