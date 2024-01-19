set(core_library_sources
        Controller/DefaultController.hpp
        Controller/LecturerController.hpp
        AppComponent.hpp
        SwaggerComponent.hpp
        ErrorHandler/ErrorHandler.hpp
        ErrorHandler/ErrorHandler.cpp
        Dto/DefaultDto.hpp
        Dto/StatusDto.hpp
        Dto/ConfigDto.hpp
        Dto/LecturerDto.hpp
        Database/DatabaseComponent.hpp
        Database/LecturerDb.hpp
        Database/Models/LecturerModel.hpp
        Config/Config.hpp
        Config/Config.cpp
        Lecturer/Lecturer.hpp
        Lecturer/Lecturer.cpp
)

add_library(${project_name}-core ${core_library_sources})

add_executable(${project_name}-exe
    App.cpp
)
# Find dependencies
find_package(oatpp 1.3.0 REQUIRED)
find_package(oatpp-postgresql 1.3.0 REQUIRED)
find_package(oatpp-swagger 1.3.0 REQUIRED)
add_compile_definitions(OATPP_SWAGGER_RES_PATH="${oatpp-swagger_INCLUDE_DIRS}/../bin/oatpp-swagger/res")
find_package(OpenSSL REQUIRED)

# Link the library to oatpp
target_link_libraries(${project_name}-core
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-postgresql
        PUBLIC oatpp::oatpp-swagger
        PUBLIC OpenSSL::SSL
)

add_compile_definitions(DEFAULT_CONFIG_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/../config/")
add_compile_definitions(DEFAULT_MIGRATIONS_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/../sql/")

target_include_directories(${project_name}-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${project_name}-exe ${project_name}-core) 
add_dependencies(${project_name}-exe ${project_name}-core)

include_directories(${OATPP_INCLUDE_DIRS})                                                                                                                                                                                                