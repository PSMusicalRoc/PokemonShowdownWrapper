find_package(SDL2 REQUIRED)

file(GLOB SERVER_SOURCE_FILES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB IMGUI_SOURCE_FILES ${CMAKE_SOURCE_DIR}/vendor/imgui/*.cpp ${CMAKE_SOURCE_DIR}/vendor/imgui/*.h)
file(GLOB IMGUI_BACKENDS ${CMAKE_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_sdl2.cpp ${CMAKE_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_sdl2.h
    ${CMAKE_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_sdlrenderer2.cpp ${CMAKE_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_sdlrenderer2.h
)
file(GLOB IMGUI_CPP_SUPPORT ${CMAKE_SOURCE_DIR}/vendor/imgui/misc/cpp/imgui_stdlib.cpp ${CMAKE_SOURCE_DIR}/vendor/imgui/misc/cpp/imgui_stdlib.h)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(../vendor/imgui)
include_directories(../vendor/imgui/backends)
include_directories(../vendor/imgui/misc/cpp)

message(SOURCE="${SERVER_SOURCE_FILES}")
message(IMGUI="${IMGUI_SOURCE_FILES}")

set(CMAKE_BUILD_TYPE Debug)
add_executable(player_client ${SERVER_SOURCE_FILES} ${IMGUI_SOURCE_FILES}
    ${IMGUI_BACKENDS} ${IMGUI_CPP_SUPPORT}
)

target_include_directories(player_client PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/PlayerPort/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(player_client ${SDL2_LIBRARIES})
