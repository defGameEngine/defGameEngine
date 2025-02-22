---@diagnostic disable: undefined-global
workspace "defGameEngine"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release"
    }

OUTPUT_DIR = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Engine/Vendor/glfw"

project "Engine"
    location "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

    targetdir ("%{wks.location}/Build/Target/" .. OUTPUT_DIR .. "/%{prj.name}")
    objdir ("%{wks.location}/Build/Obj/" .. OUTPUT_DIR .. "/%{prj.name}")

    -- Linking with GLFW3

    links { "GLFW3" }

    -- Setting up precompiled headers

    pchheader "Pch.hpp"
    pchsource "%{prj.name}/Sources/Pch.cpp"

    -- Including all source and header files of the engine

    files
    {
        "%{prj.name}/Include/*.hpp",
        "%{prj.name}/Sources/*.cpp",
        "%{prj.name}/Sources/*.inl"
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
        "%{prj.name}/Vendor/glfw/include",
        "%{prj.name}/Vendor/stb",
        "%{prj.name}/Include",
        "%{prj.name}/Sources",
    }

    -- Linking with libraries

    filter "system:windows"
        links { "gdi32", "user32", "kernel32", "opengl32", "glu32" }

    filter "system:linux"
        links
        {
            "GL", "GLU", "glut", "GLEW", "X11",
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
        "{COPY} %{cfg.buildtarget.relpath} \"%{wks.location}/Build/Target/" .. OUTPUT_DIR .. "/Sandbox/\""
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

    targetdir ("%{wks.location}/Build/Target/" .. OUTPUT_DIR .. "/%{prj.name}")
    objdir ("%{wks.location}/Build/Obj/" .. OUTPUT_DIR .. "/%{prj.name}")

    -- Link projects

    links { "GLFW3", "Engine" }

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
        "Engine/Vendor/glfw/include",
        "Engine/Vendor/stb",
        "Engine/Include"
    }

    -- Linking with libraries

    libdirs { "Engine/Vendor/glfw/%{cfg.architecture}" }

    filter "system:windows"
        links { "gdi32", "user32", "kernel32", "opengl32", "GLFW3", "glu32" }

    filter "system:linux"
        links
        {
            "GL", "GLU", "glut", "GLEW", "GLFW3", "X11",
            "Xxf86vm", "Xrandr", "pthread", "Xi", "dl",
            "Xinerama", "Xcursor"
        }

    -- Platform specific flags

    filter "system:windows"
        warnings "Extra"

    filter {}

    -- Build configurations

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"

    filter {}
