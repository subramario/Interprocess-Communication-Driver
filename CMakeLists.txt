cmake_minimum_required(VERSION 2.8)
project(IPC_Driver)

## Project specific configurations go here

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/run)

# create the main executable - add additional .cpp files if needed
add_executable(random_street random_street_generator.cpp )
add_executable(shortest shortest_path_deriver.cpp )
add_executable(IPC IPC_driver.cpp )

install(PROGRAMS undirected_graph_generator.py DESTINATION bin)
install(TARGETS random_street shortest IPC RUNTIME DESTINATION bin)
