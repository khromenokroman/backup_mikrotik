cmake_minimum_required(VERSION 3.18.4)
project(mikrotik_backup
        VERSION 0.0.1
        LANGUAGES CXX
        HOMEPAGE_URL "https://github.com/khromenokroman/backup_mikrotik")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
find_package(fmt REQUIRED)
pkg_check_modules(LIBSSH2 REQUIRED libssh2)

include(FetchContent)

FetchContent_Declare(
        ssh_client
        GIT_REPOSITORY https://github.com/khromenokroman/ssh_client.git
        GIT_TAG master
)

FetchContent_MakeAvailable(ssh_client)


add_executable(${PROJECT_NAME}
        src/main.cpp
        src/backup.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE
        ssh-client-lib
        ${LIBSSH2_LIBRARIES}
        fmt::fmt
)

add_subdirectory(install)
add_subdirectory(deb)