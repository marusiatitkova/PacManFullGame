cmake_minimum_required(VERSION 3.15)
project(PacManFullGame)

set(CMAKE_CXX_STANDARD 14)
set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS window graphics audio system REQUIRED)

add_executable(PacManFullGame
        src/main.cpp
        src/Pacman.cpp include/Pacman.h
        src/Game_window.cpp include/Game_window.h
        src/Maze.cpp include/Maze.h
        src/Cell.cpp include/Cell.h
        src/Path_algorithms.cpp include/Path_algorithms.h src/Ghost.cpp include/Ghost.h src/GhostController.cpp include/GhostController.h src/PacmanController.cpp include/PacmanController.h)

target_link_libraries(PacManFullGame sfml-graphics sfml-audio sfml-window sfml-system)