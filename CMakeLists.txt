cmake_minimum_required(VERSION 3.18.0)
project(RushHour LANGUAGES CXX)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(-Wall -Wpedantic)
endif()

find_package(SFML COMPONENTS graphics window system REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(RushHour 
    src/main.cpp 
    src/Grille.cpp 
    src/Vehicule.cpp
    src/Solver.cpp
)

target_link_libraries(RushHour PRIVATE sfml-graphics sfml-window sfml-system)

target_compile_features(RushHour PRIVATE cxx_std_17)
