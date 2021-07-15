cmake_minimum_required(VERSION 3.19)
project(roguelitegame)

set(CMAKE_CXX_STANDARD 14)
set(EXE_NAME game)
set(CMAKE_MODULE_PATH "C:/dev/CMake/Module")
set(SDL2_PATH "C:/dev/SDL2-2.0.14/x86_64-w64-mingw32")

find_package(SDL2)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(include/libtcod-1.16.6-x86-mingw/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

file(GLOB TCODINC include/libtcod-1.16.6-x86-mingw/include/*.h include/libtcod-1.16.6-x86-mingw/include/*.hpp)
file(GLOB SRC src/*.hpp src/*.cpp)

set(SOURCE_FILES ${SRC} ${TCODINC})

add_executable(${EXE_NAME} ${SOURCE_FILES})

target_link_libraries(${EXE_NAME} ${SDL2_LIBRARY} tcod)
