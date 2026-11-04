cmake_minimum_required(VERSION 3.22)
project(acs LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
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

find_package(libpqxx CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE libpqxx::pqxx)

find_package(httplib CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE httplib::httplib)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)