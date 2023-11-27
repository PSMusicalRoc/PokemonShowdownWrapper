find_package(SDL2 REQUIRED)

file(GLOB SERVER_SOURCE_FILES src/*.cpp)
include_directories(${SDL2_INCLUDE_DIRS})

set(CMAKE_BUILD_TYPE Debug)
add_executable(player_client ${SERVER_SOURCE_FILES})
target_include_directories(player_client PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/PlayerPort/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(player_client ${SDL2_LIBRARIES})
