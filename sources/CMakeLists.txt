cmake_minimum_required(VERSION 2.8)

project(mpxx)

set(CMAKE_RULE_MESSAGES OFF)
set(CMAKE_COLOR_MAKEFILE OFF)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_EXE_LINKER_FLAGS "-L/usr/pkg/lib ")
set(CMAKE_SHARED_LINKER_FLAGS "-L/usr/pkg/lib ")
set(CMAKE_MODULE_LINKER_FLAGS "-L/usr/pkg/lib")
set(CMAKE_CXX_FLAGS "-g -pipe -Wall -fvisibility=hidden -fPIC -fstack-protector-all -O3 -march=native -std=c++11 -stdlib=libc++ -Qunused-arguments -fcolor-diagnostics -ftemplate-depth=256 -I/usr/pkg/include")
set(CMAKE_C_FLAGS "-g -pipe -Wall -fvisibility=hidden -fPIC -fstack-protector-all -O3 -march=native -Qunused-arguments -fcolor-diagnostics -I/usr/pkg/include")

add_definitions(
    -DMPXX_EXPORTS
    -DUNIX
    -DDARWIN
    -D_FORTIFY_SOURCE=2
)

include_directories("../build/include.generated")

install(
    DIRECTORY "../build/include.generated/"
    DESTINATION include/mpxx
)

install(
    DIRECTORY "include/mpxx/"
    DESTINATION include/mpxx
)

install(
    DIRECTORY "../build/pkgconfig/"
    DESTINATION lib/pkgconfig
)

install(
    DIRECTORY etc/
    DESTINATION /etc/mpxx
    OPTIONAL
)

install(
    DIRECTORY etc_/
    DESTINATION /etc
    OPTIONAL
)

install(
    DIRECTORY share/
    DESTINATION share/mpxx
    OPTIONAL
)

install(
    DIRECTORY share_/
    DESTINATION share
    OPTIONAL
)

install(
    PROGRAMS ../build/bin/mpxx-config
    DESTINATION bin
)

add_custom_target(
    mpxx_config_man
    ALL
    COMMAND pod2man -n mpxx-config ../build/bin/mpxx-config ../build/man/mpxx-config.1
)

install(
    DIRECTORY ../build/man/
    DESTINATION man/man1
    OPTIONAL
)

add_subdirectory(tests/mpxx_msg)
