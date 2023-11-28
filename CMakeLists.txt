set(CMAKE_INSTALL_PREFIX .)

cmake_minimum_required(VERSION 3.27)
project(NativePokemonGame
    VERSION 0.0.0)
set(CXX_STANDARD 11)

add_subdirectory(Server)
add_subdirectory(PlayerPort)

install(TARGETS server)
install(TARGETS player_client)
install(DIRECTORY vendor/nodejs
    DESTINATION bin/vendor
    USE_SOURCE_PERMISSIONS
)
install(DIRECTORY vendor/pokemon-showdown
    DESTINATION bin/vendor
    USE_SOURCE_PERMISSIONS
)
install(DIRECTORY res/
    DESTINATION bin
    USE_SOURCE_PERMISSIONS
)
install(PROGRAMS Server/run_server
    DESTINATION bin
)