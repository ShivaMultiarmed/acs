cmake_minimum_required(VERSION 4.3)
set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
project(acs LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SKIP_BUILD_TEST ON CACHE BOOL "" FORCE)
set(SKIP_PQXX_SHARED ON CACHE BOOL "" FORCE)
add_subdirectory(third_party/libpqxx)

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
        service/InteractionService.h)
target_link_libraries(${PROJECT_NAME} PRIVATE pqxx_static)