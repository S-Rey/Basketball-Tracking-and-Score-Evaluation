cmake_minimum_required(VERSION 3.3)
project(Projet)
find_package(OpenCV REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        CMakeLists.txt
        Vertex.cpp
        Vertex.h
        Zone.cpp
        Zone.h
        Keyframe.cpp
        Keyframe.h
        Video.cpp
        Video.h
        KFilter.cpp
        KFilter.h
        Functions.cpp
        Functions.h
        Line.cpp
        Line.h
        Parabola.cpp
        Parabola.h
        main.cpp)

add_executable(Projet ${SOURCE_FILES})

target_link_libraries(Projet ${OpenCV_LIBS})

