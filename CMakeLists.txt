cmake_minimum_required(VERSION 3.16)

set(PATCH_VERSION "0" CACHE STRING "Patch version")
project(physics VERSION 1.0.${PATCH_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.6 COMPONENTS graphics QUIET)

if(NOT SFML_FOUND)
    include(FetchContent)

    set(CMAKE_POLICY_VERSION_MINIMUM 3.10)
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(WARNINGS_AS_ERRORS FALSE CACHE BOOL "" FORCE)

    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.2
    )
    FetchContent_MakeAvailable(SFML)
endif()

add_executable(physics
    main.cpp
    World.cpp
    Ball.cpp
    Dust.cpp
    Physics.cpp
    Color.cpp
    Painter.cpp
    black_box/Application.cpp
    black_box/View.cpp
    black_box/PainterImpl.cpp
)

target_link_libraries(physics PRIVATE sfml-graphics)

target_compile_options(physics PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

if(WIN32)
    install(TARGETS physics RUNTIME DESTINATION . COMPONENT PhysicsRuntime)
    install(DIRECTORY data DESTINATION . COMPONENT PhysicsRuntime)
else()
    install(TARGETS physics RUNTIME DESTINATION bin COMPONENT PhysicsRuntime)
    install(DIRECTORY data DESTINATION share/physics COMPONENT PhysicsRuntime)
endif()

set(CPACK_PACKAGE_CONTACT "plum591")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
include(CPack)