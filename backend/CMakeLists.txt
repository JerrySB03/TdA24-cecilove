cmake_minimum_required(VERSION 3.11)

set(project_name cecilsky-backend) ## setting the project name

project(${project_name} CXX)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)



add_definitions(
        ## Cmake build type
        -DCMAKE_BUILD_TYPE=Debug

)
add_subdirectory(src)
add_subdirectory(lib)

set_target_properties(${project_name}-core ${project_name}-exe PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)