set(CMAKE_INSTALL_PREFIX .)

cmake_minimum_required(VERSION 3.27)
project(NativePokemonGame
    VERSION 0.0.0)
set(CXX_STANDARD 11)

add_subdirectory(Server)

install(TARGETS server)
install(DIRECTORY vendor
    DESTINATION bin
    USE_SOURCE_PERMISSIONS
)
install(PROGRAMS Server/run_server
    DESTINATION bin
)