set(core_library_sources
        Controller/DefaultController.hpp
        AppComponent.hpp
        SwaggerComponent.hpp
        ErrorHandler/ErrorHandler.hpp
        ErrorHandler/ErrorHandler.cpp
        Dto/DefaultDto.hpp
        Dto/StatusDto.hpp
        Dto/ConfigDto.hpp
        Database/DatabaseComponent.hpp
        Config/Config.hpp
        Config/Config.cpp
)

add_library(${project_name}-core ${core_library_sources})

add_executable(${project_name}-exe
    App.cpp
)

# Link the library to oatpp
target_link_libraries(${project_name}-core
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-sqlite
        PUBLIC oatpp::oatpp-swagger
        PUBLIC argon2
)

add_compile_definitions(DEFAULT_CONFIG_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/../config/")
add_compile_definitions(DEFAULT_MIGRATIONS_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/../sql/")

target_include_directories(${project_name}-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${project_name}-exe ${project_name}-core) 
add_dependencies(${project_name}-exe ${project_name}-core)

include_directories(${OATPP_INCLUDE_DIRS})                                                                                                                                                                                                