cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_STATIC_LIBRARIES TRUE)

project(tictactoe VERSION 1.0)

find_package(SFML 3 COMPONENTS Graphics Window System Audio Network REQUIRED) 

add_executable(tictactoe main.cpp)

target_link_libraries(tictactoe SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)

add_custom_command(TARGET tictactoe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/img $<TARGET_FILE_DIR:tictactoe>/img)