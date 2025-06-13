cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(tictactoe VERSION 1.0)

find_package(SFML 3 COMPONENTS Graphics Window System Audio Network REQUIRED) 

add_executable(tictactoe main.cpp)

target_link_libraries(tictactoe sfml-graphics sfml-window sfml-system sfml-audio sfml-network)

add_custom_command(TARGET tictactoe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/img $<TARGET_FILE_DIR:tictactoe>/img)