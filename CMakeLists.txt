cmake_minimum_required(VERSION 3.22)
project(acs LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp
        repository/PermissionRepository.cpp
        repository/PermissionRepository.h
        repository/WorkerRepository.cpp
        repository/WorkerRepository.h
        repository/InteractionRepository.cpp
        repository/InteractionRepository.h
        utils.h
        utils.cpp
        service/InteractionService.cpp
        service/InteractionService.h
        endpoints/interaction_endpoints.h
        endpoints/interactions_endpoints.cpp
        repository/ObjectRepository.cpp
        repository/ObjectRepository.h)

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

find_package(PkgConfig MODULE REQUIRED)
pkg_check_modules(PQXX REQUIRED IMPORTED_TARGET libpqxx)
pkg_check_modules(HTTPLIB REQUIRED IMPORTED_TARGET cpp-httplib)
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::PQXX PkgConfig::HTTPLIB)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)