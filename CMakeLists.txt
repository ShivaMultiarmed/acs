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

if(WIN32)
    # локально: vcpkg + libpqxx из исходников
    add_subdirectory(third_party/libpqxx)
    target_link_libraries(${PROJECT_NAME} PRIVATE pqxx_static)

    find_package(httplib CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE httplib::httplib)
else()
    # Docker / Ubuntu: системные пакеты через pkg-config
    find_package(Threads REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PQXX REQUIRED IMPORTED_TARGET libpqxx)
    pkg_check_modules(HTTPLIB REQUIRED IMPORTED_TARGET cpp-httplib)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            PkgConfig::PQXX PkgConfig::HTTPLIB Threads::Threads)
endif()

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)