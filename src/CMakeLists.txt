cmake_minimum_required(VERSION 3.7)
project(main VERSION 0.3 LANGUAGES CXX)
set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/lib/imgui)
set(IMGUI_SFML_FIND_SFML OFF)
add_subdirectory(lib)
add_subdirectory(src)