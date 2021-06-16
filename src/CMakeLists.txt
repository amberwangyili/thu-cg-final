cmake_minimum_required(VERSION 3.7)
#construct target
add_executable(kdtree main.cpp)
#add compile options
target_compile_features(kdtree PRIVATE cxx_std_17)
#include path of the public header files 
find_package(OpenGL REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
target_include_directories(kdtree 
                            PRIVATE ${PROJECT_SOURCE_DIR}/include
                            PRIVATE ${PROJECT_SOURCE_DIR}/lib/imgui
                            PRIVATE ${PROJECT_SOURCE_DIR}/lib/imgui-sfml
                            PRIVATE ${OpenGL_INCLUDE_DIRS}
                            PRIVATE ${SFML_INCLUDE_DIRS})

target_link_libraries(kdtree
                            PRIVATE ImGui-SFML
                            PRIVATE sfml-graphics sfml-window sfml-system)               






