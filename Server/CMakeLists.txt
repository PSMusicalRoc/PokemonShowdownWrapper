file(GLOB SERVER_SOURCE_FILES src/*.cpp)

set(CMAKE_BUILD_TYPE Debug)
add_executable(server ${SERVER_SOURCE_FILES})
target_include_directories(server PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Server/include>
    $<INSTALL_INTERFACE:include>
)
