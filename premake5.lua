---@diagnostic disable: undefined-global

newoption
{
    trigger = "backend",
    value = "API",
    description = "Choose backend",
    allowed =
    {
        { "sdl"  },
        { "glfw" }
    }
}

newoption
{
    trigger = "need-audio",
    description = "Provide audio part"
}

workspace "defGameEngine"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release"
    }

    filter "system:windows or system:linux"
        architecture "x64"

    filter "system:macosx"
        architecture "ARM64"

    filter {}

OUTPUT_DIR = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

local BACKEND = _OPTIONS["backend"] or "glfw"
local NEED_AUDIO = _OPTIONS["need-audio"] or false

local USE_GLFW3 = BACKEND == "glfw"
local USE_SDL3 = BACKEND == "sdl"

if USE_GLFW3 then
    defines { "DGE_PLATFORM_GLFW3" }
elseif USE_SDL3 then
    defines { "DGE_PLATFORM_SDL3" }
else
    print("No platform selected")
    os.exit(1)
end

if NEED_AUDIO then
    defines { "DGE_NEED_AUDIO" }
end

if USE_GLFW3 then
    include "Engine/Vendor/glfw"
elseif USE_SDL3 then
    if not os.isdir("Engine/Vendor/SDL/build") then
        local sdl_dir = "Engine/Vendor/SDL"
        local build_dir = sdl_dir .. "/build"
        local install_dir = sdl_dir .. "/install"
        
        print("Building SDL3...")
        
        os.mkdir(build_dir)
        os.mkdir(install_dir)
        
        local current_dir = os.getcwd()
        os.chdir(sdl_dir)
        
        if os.isdir("build") then
            os.rmdir("build")
        end

        os.mkdir("build")
        
        local cmake_config = string.format(
            'cmake -S . -B build ' ..
            '-DCMAKE_INSTALL_PREFIX="%s/install" ' ..
            '-DBUILD_SHARED_LIBS=OFF ' ..
            '-DSDL_STATIC=ON ' ..
            '-DSDL_TEST=OFF ' ..
            '-DSDL_EXAMPLES=OFF ' ..
            '-DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreaded$<$<CONFIG:Debug>:Debug>"',
            os.getcwd()
        )
        
        print("Configuring SDL3...")
        os.execute(cmake_config)
        
        print("Building SDL3 Release...")
        os.execute("cmake --build build --config Release")
        os.execute("cmake --install build --config Release")
        
        print("Building SDL3 Debug...")
        os.execute("cmake --build build --config Debug")
        os.execute("cmake --install build --config Debug")
        
        os.chdir(current_dir)
        
        print("SDL3 built successfully!")
    end
end

project "Engine"
    location "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

    targetdir ("%{wks.location}/Build/Target/" .. OUTPUT_DIR .. "/%{prj.name}")
    objdir ("%{wks.location}/Build/Obj/" .. OUTPUT_DIR .. "/%{prj.name}")

    staticruntime "On"

    if USE_GLFW3 then
        defines { "DGE_PLATFORM_GLFW3" }
        links { "GLFW3" }
    elseif USE_SDL3 then
        defines { "DGE_PLATFORM_SDL3" }
        links { "SDL3-static" }
        libdirs { "Engine/Vendor/SDL/install/lib" }
    end

    pchheader "Pch.hpp"
    pchsource "%{prj.name}/Sources/Pch.cpp"

    files
    {
        "%{prj.name}/Include/*.hpp",
        "%{prj.name}/Sources/*.cpp"
    }

    if NEED_AUDIO then
        includedirs { "%{prj.name}/Vendor/miniaudio" }
    else
        removefiles
        {
            "%{prj.name}/Include/Audio.hpp",
            "%{prj.name}/Sources/Audio.cpp",
            "%{prj.name}/Sources/StbVorbis.cpp"
        }
    end

    removefiles { "%{prj.name}/Sources/Utils.cpp" }

    if USE_GLFW3 then
        filter { "system:windows or system:linux or system:macosx" }
            removefiles
            {
                "%{prj.name}/Include/PlatformEmscripten.hpp",
                "%{prj.name}/Sources/PlatformEmscripten.cpp",
                "%{prj.name}/Include/PlatformSDL3.hpp",
                "%{prj.name}/Sources/PlatformSDL3.cpp"
            }

        filter "system:emscripten"
            removefiles
            {
                "%{prj.name}/Include/PlatformGLFW3.hpp",
                "%{prj.name}/Sources/PlatformGLFW3.cpp"
            }
    elseif USE_SDL3 then
        filter { "system:windows or system:linux or system:macosx" }
            removefiles
            {
                "%{prj.name}/Include/PlatformEmscripten.hpp",
                "%{prj.name}/Sources/PlatformEmscripten.cpp",
                "%{prj.name}/Include/PlatformGLFW3.hpp",
                "%{prj.name}/Sources/PlatformGLFW3.cpp"
            }

        filter "system:emscripten"
            removefiles
            {
                "%{prj.name}/Include/PlatformSDL3.hpp",
                "%{prj.name}/Sources/PlatformSDL3.cpp"
            }
    end

    filter {}

    local dirs = {
        "%{prj.name}/Vendor/stb",
        "%{prj.name}/Include",
        "%{prj.name}/Sources",
    }
    
    if USE_GLFW3 then
        table.insert(dirs, "%{prj.name}/Vendor/glfw/include")
    elseif USE_SDL3 then
        table.insert(dirs, "%{prj.name}/Vendor/SDL/include")
        table.insert(dirs, "%{prj.name}/Vendor/SDL/install/include/SDL3")
    end

    if NEED_AUDIO then
        table.insert(dirs, "%{prj.name}/Vendor/miniaudio")
    end
    
    includedirs (dirs)

    filter "system:windows"
        links
        { 
            "gdi32", "user32", "kernel32", "opengl32", "glu32",
            "winmm", "imm32", "version", "setupapi",
            "legacy_stdio_definitions.lib"
        }

    filter "system:linux"
        links
        {
            "GL", "GLU", "glut", "GLEW", "X11",
            "Xxf86vm", "Xrandr", "pthread", "Xi", "dl",
            "Xinerama", "Xcursor", "m"
        }

    filter "system:macosx"
        links
        {
            "Metal.framework", "QuartzCore.framework",
            "Cocoa.framework", "OpenGL.framework",
            "IOKit.framework", "CoreVideo.framework"
        }

    filter "system:windows"
        warnings "Extra"
        systemversion "latest"

    filter {}

    postbuildcommands
    {
        "{COPY} %{cfg.buildtarget.relpath} \"%{wks.location}/Build/Target/" .. OUTPUT_DIR .. "/Sandbox/\""
    }

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

    targetdir ("%{wks.location}/Build/Target/" .. OUTPUT_DIR .. "/%{prj.name}")
    objdir ("%{wks.location}/Build/Obj/" .. OUTPUT_DIR .. "/%{prj.name}")

    if USE_GLFW3 then
        defines { "DGE_PLATFORM_GLFW3" }
    elseif USE_SDL3 then
        defines { "DGE_PLATFORM_SDL3" }
    end

    local links_list = { "Engine" }
    
    if USE_GLFW3 then
        table.insert(links_list, "GLFW3")
    elseif USE_SDL3 then
        table.insert(links_list, "SDL3-static")
    end
    
    links (links_list)

    files
    {
        "%{prj.name}/Include/*.hpp",
        "%{prj.name}/Sources/*.cpp"
    }

    local sandbox_includedirs = {
        "Engine/Vendor/stb",
        "Engine/Include"
    }
    
    if USE_GLFW3 then
        table.insert(sandbox_includedirs, "Engine/Vendor/glfw/include")
    elseif USE_SDL3 then
        table.insert(sandbox_includedirs, "Engine/Vendor/SDL/include")
        table.insert(sandbox_includedirs, "Engine/Vendor/SDL/install/include/SDL3")
    end

    if NEED_AUDIO then
        table.insert(sandbox_includedirs, "Engine/Vendor/miniaudio")
    end
    
    includedirs (sandbox_includedirs)

    if USE_GLFW3 then
        libdirs { "Build/Target/" .. OUTPUT_DIR .. "/GLFW3" }
    elseif USE_SDL3 then
        libdirs { "Engine/Vendor/SDL/install/lib" }
    end

    filter "system:windows"
        local windows_links = {
            "gdi32", "user32", "kernel32", "opengl32", "glu32",
            "winmm", "imm32", "version", "setupapi",
            "legacy_stdio_definitions.lib"
        }

        if USE_GLFW3 then
            table.insert(windows_links, "GLFW3")
        elseif USE_SDL3 then
            table.insert(windows_links, "SDL3-static")
        end

        links (windows_links)
        
        buildoptions { "/D _CRT_SECURE_NO_WARNINGS" }

    filter "system:linux"
        local linux_links = {
            "GL", "GLU", "glut", "GLEW", "X11",
            "Xxf86vm", "Xrandr", "pthread", "Xi", "dl",
            "Xinerama", "Xcursor", "m"
        }

        if USE_GLFW3 then
            table.insert(linux_links, "GLFW3")
        elseif USE_SDL3 then
            table.insert(linux_links, "SDL3-static")
        end

        links (linux_links)

    filter "system:macosx"
        local macos_links = {
            "Metal.framework", "QuartzCore.framework",
            "Cocoa.framework", "OpenGL.framework",
            "IOKit.framework", "CoreVideo.framework"
        }

        if USE_SDL3 then
            table.insert(macos_links, "SDL3-static")
        end

        links (macos_links)
        
        if not USE_GLFW3 then
            buildoptions { "-ObjC++" }
            toolset "clang"
        end

    filter "system:windows"
        warnings "Extra"

    filter {}

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"

    filter {}