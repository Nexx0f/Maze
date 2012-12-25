project(maze)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

# set(maze_SRCS maze.cpp main.cpp automata.cpp)
set(maze_SRCS maze.cpp main.cpp)

qt4_automoc(${maze_SRCS})
add_executable(maze ${maze_SRCS})
target_link_libraries(maze ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
