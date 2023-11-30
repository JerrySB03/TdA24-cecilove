cmake_minimum_required(VERSION 3.11)

set(project_name cecilsky-backend) ## setting the project name

project(${project_name} CXX)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)

# Find dependencies
find_package(oatpp 1.3.0 REQUIRED)
# Sqlite needs threads on Linux
if(CMAKE_SYSTEM_NAME MATCHES Linux)
    find_package(Threads REQUIRED)
endif()
find_package(SQLite3 REQUIRED)
find_package(oatpp-sqlite 1.3.0 REQUIRED)
find_package(oatpp-swagger 1.3.0 REQUIRED)

add_definitions(
        ## define path to swagger-ui static resources folder
        -DOATPP_SWAGGER_RES_PATH="${oatpp-swagger_INCLUDE_DIRS}/../bin/oatpp-swagger/res"

        ## Cmake build type
        -DCMAKE_BUILD_TYPE="DEBUG"

)
add_subdirectory(src)
add_subdirectory(lib)

set_target_properties(${project_name}-core ${project_name}-exe PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)