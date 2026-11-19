# Было: минимальная версия CMake 3.7.2.
# Стало: 3.16 для используемой команды FetchContent_MakeAvailable.
cmake_minimum_required(VERSION 3.16)

# Было: project(physics) без номера версии.
# Стало: версия 1.0.N, где N в GitHub Actions берётся из числа коммитов.
set(PATCH_VERSION "0" CACHE STRING "Patch version")
project(physics VERSION 1.0.${PATCH_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
# Дополнительно требуем, чтобы компилятор поддерживал C++17.
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Было: поиск SFML без указания версии, при загрузке использовался тег 2.6.x.
# Стало: поиск совместимой SFML 2.6, для загрузки закрепили тег 2.6.2.
find_package(SFML 2.6 COMPONENTS graphics QUIET)

if(NOT SFML_FOUND)
    include(FetchContent)

    # Добавили настройку для обработки старого CMake-файла SFML новым CMake.
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

# Было: использовалась настройка для M_PI на MSVC.
# Стало: pi объявлена в Ball.cpp, поэтому настройка M_PI убрана.

# Было: в список входили только исходники заготовки.
# Стало: в physics компилируется и новый Dust.cpp.
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

# Было: проект только собирал исполняемый файл.
# Стало: install кладёт в пакет программу и папку с моделями.
if(WIN32)
    install(TARGETS physics RUNTIME DESTINATION . COMPONENT PhysicsRuntime)
    install(DIRECTORY data DESTINATION . COMPONENT PhysicsRuntime)
else()
    install(TARGETS physics RUNTIME DESTINATION bin COMPONENT PhysicsRuntime)
    install(DIRECTORY data DESTINATION share/physics COMPONENT PhysicsRuntime)
endif()

# Было: правил упаковки DEB не было.
# Стало: CPack создаёт DEB и определяет зависимости от динамических библиотек.
set(CPACK_PACKAGE_CONTACT "plum591")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
include(CPack)