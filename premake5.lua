---@diagnostic disable: undefined-global
workspace "defGameEngine"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release"
    }

local OUTPUT_DIR = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Engine"
    location "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

    targetdir ("Build/Target/" .. OUTPUT_DIR .. "/%{prj.name}")
    objdir ("Build/Obj/" .. OUTPUT_DIR .. "/%{prj.name}")

    -- Including all source and header files of the engine

    files
    {
        "%{prj.name}/Include/*.hpp",
        "%{prj.name}/Sources/*.cpp"
    }

    filter { "system:windows or system:linux" }
        removefiles
        {
            "%{prj.name}/Include/PlatformEmscripten.hpp",
            "%{prj.name}/Sources/PlatformEmscripten.cpp"
        }

    filter "system:emscripten"
        removefiles
        {
            "%{prj.name}/Include/PlatformGLFW3.hpp",
            "%{prj.name}/Sources/PlatformGLFW3.cpp"
        }

    filter {}

    -- Including headers for libraries

    includedirs
    {
        "%{prj.name}/Vendor/glfw3/include",
        "%{prj.name}/Vendor/stb",
        "%{prj.name}/Include"
    }

    -- Linking with libraries
    
    libdirs { "%{prj.name}/Vendor/glfw3/%{cfg.architecture}" }

    filter "system:windows"
        links { "gdi32", "user32", "kernel32", "opengl32", "glfw3dll", "glu32" }

    filter "system:linux"
        links
        {
            "GL", "GLU", "glut", "GLEW", "glfw", "X11",
            "Xxf86vm", "Xrandr", "pthread", "Xi", "dl",
            "Xinerama", "Xcursor"
        }

    -- Platform specific flags

    filter "system:windows"
        warnings "Extra"
        staticruntime "On"
        systemversion "latest"

    filter {}

    -- Puts the engine's .dll file near the Sandbox executable

    postbuildcommands
    {
        "{COPY} %{cfg.buildtarget.relpath} \"../Build/Target/" .. OUTPUT_DIR .. "/Sandbox/\""
    }

    -- Build configurations

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"

    filter {}

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "On"
    systemversion "latest"

    targetdir ("Build/Target/" .. OUTPUT_DIR .. "/%{prj.name}")
    objdir ("Build/Obj/" .. OUTPUT_DIR .. "/%{prj.name}")

    -- Links

    links { "Engine" }

    -- Including all source and header files of the engine

    files
    {
        "%{prj.name}/Include/*.hpp",
        "%{prj.name}/Sources/*.cpp"
    }

    filter { "system:windows or system:linux" }
        removefiles { "Engine/Include/PlatformEmscripten.hpp" }

    filter "system:emscripten"
        removefiles { "Engine/Include/PlatformGLFW3.hpp" }

    filter {}

    -- Including headers for libraries

    includedirs
    {
        "Engine/Vendor/glfw3/include",
        "Engine/Vendor/stb",
        "Engine/Include",
    }

    -- Linking with libraries

    libdirs { "Engine/Vendor/glfw3/%{cfg.architecture}" }

    filter "system:windows"
        links { "gdi32", "user32", "kernel32", "opengl32", "glfw3dll", "glu32" }

    filter "system:linux"
        links
        {
            "GL", "GLU", "glut", "GLEW", "glfw", "X11",
            "Xxf86vm", "Xrandr", "pthread", "Xi", "dl",
            "Xinerama", "Xcursor"
        }

    -- Platform specific flags

    filter "system:windows"
        warnings "Extra"

    filter {}

    -- Puts glfw3.dll near the Sandbox.exe

    postbuildcommands
    {
        "{COPY} ../Engine/Vendor/glfw3/x86_64/glfw3.dll \"../Build/Target/" .. OUTPUT_DIR .. "/Sandbox/\""
    }

    -- Build configurations

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"

    filter {}
