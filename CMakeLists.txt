cmake_minimum_required(VERSION 3.20)
project(Lab-work-6 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W3)
    add_compile_options(/EHsc)
    add_compile_options(/utf-8)
else()
    add_compile_options(-Wall)
    add_compile_options(-fexceptions)
endif()

add_executable(Lab-work-6
    gameRules.cpp
    randomStrategy.cpp
    playerTask.cpp
    game.cpp
    reader.cpp
    main.cpp
)

target_include_directories(Lab-work-6 PRIVATE include)