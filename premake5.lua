workspace "ShowdownWrapper"
    configurations { "Debug", "Release" }

project "ShowdownWrapper"
    kind "ConsoleApp"
    language "C++"
    
files {
    "src/**.cpp"
}

includedirs {
    "include"
}

libdirs {
    "lib"
}

filter "configurations:Debug"
    symbols "On"
    defines { "ROC_DEBUG" }

filter "configurations:Release"
    optimize "On"
