local CustomAttributes = {};
CustomAttributes.IncludeDirectories = {
    "vendor/imgui/backends",
    "vendor/imgui/misc/cpp",
    "vendor/imgui",
    "vendor/nlohmannjson"
};

CustomAttributes.ImGuiFiles = {
    "vendor/imgui/*.cpp",
    "vendor/imgui/backends/imgui_impl_sdl2.cpp",
    "vendor/imgui/backends/imgui_impl_sdlrenderer2.cpp",
    "vendor/imgui/misc/cpp/imgui_stdlib.cpp"
};

local pwd = path.getabsolute(".")

workspace "PokemonShowdownWrapper"
    configurations { "Debug", "Release" }

project "server"
    language "C++"
    kind "WindowedApp"
    targetdir "build/%{cfg.longname}"

    files {
        "Server/src/**.cpp"
    }

    includedirs {
        "Server/include"
    }

filter ""

project "player-client"
    language "C++"
    kind "WindowedApp"
    dependson { "build-sdl2", "build-sdl2_image" }
    targetdir "build/%{cfg.longname}"

    files {
        "PlayerPort/src/**.cpp",
        CustomAttributes.ImGuiFiles
    }

    includedirs {
        "include",
        "include/SDL2",
        "PlayerPort/include",
        CustomAttributes.IncludeDirectories
    }

    libdirs {
        "lib"
    }

    links {
        "SDL2",
        "SDL2_image"
    }

filter "configurations:Debug"
    symbols "On"

filter "configurations:Release"
    optimize "On"

filter ""

project "build-sdl2"
    kind "Makefile"

buildcommands {
    string.format("mkdir -p vendor/SDL2/build && cd vendor/SDL2/build && ../configure --prefix=%s && make && make install", pwd)
}

project "build-sdl2_image"
    kind "Makefile"

buildcommands {
    string.format("mkdir -p vendor/SDL2_image/build && cd vendor/SDL2_image/build && ../configure --prefix=%s && make && make install", pwd)
}

project "install"
    kind "Makefile"
    dependson { "server", "player-client" }

buildcommands {
    "mkdir -p build/%{cfg.longname}/vendor",
    "cp -r res build/%{cfg.longname}",
    "cp -r vendor/pokemon-showdown build/%{cfg.longname}/vendor",
    "cp -r vendor/nodejs build/%{cfg.longname}/vendor",
    "cp Server/run_server build/%{cfg.longname}"
}