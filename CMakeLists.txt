cmake_minimum_required(VERSION 3.25.1)
project(mikrotik_backup)

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



add_executable(mikrotik_backup main.cpp)

target_link_libraries(mikrotik_backup PRIVATE
        ssh-client-lib
        ${LIBSSH2_LIBRARIES}
        fmt::fmt
)