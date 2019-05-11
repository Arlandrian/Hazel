workspace "Hazel"
    architecture "x64"

    configurations{
        "Debug",
        "Release",
        "Dist"
    }

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Hazel"
    location "Hazel"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. output_dir .. "/%{prj.name}")
    objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

    pchheader "hzpch.h"
    pchsource "Hazel/src/hzpch.cpp"

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        defines{
            "HZ_PLATFORM_WINDOWS",
            "HZ_BUILD_DLL"
        }
        
        postbuildcommands{
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. output_dir .. "/SandBox")
        }
    
    filter "configurations:Debug"
        defines "HZ_DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "HZ_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "HZ_DIST"
        optimize "On"
    
project "SandBox"
    location "SandBox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. output_dir .. "/%{prj.name}")
    objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "Hazel/vendor/spdlog/include",
        "Hazel/src",
		"Hazel/vendor"
    }

    links{
        "Hazel"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        defines{
            "HZ_PLATFORM_WINDOWS"
        }

    
    filter "configurations:Debug"
        defines "HZ_DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "HZ_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "HZ_DIST"
        optimize "On"
--SET your project as startup project