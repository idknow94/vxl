workspace "vxl"
    configurations { "Debug", "Release" }
    startproject "vxltest"

project "vxl"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir "bin/%{cfg.system}/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.system}/%{cfg.buildcfg}"

    files {
        "vxl/src/**.c",
        "vxl/include/**.h"
    }

    includedirs {
        "vxl/include"
    }

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"


project "vxltest"
    kind "ConsoleApp"
    language "C"
    staticruntime "on"

    targetdir "bin/%{cfg.system}/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.system}/%{cfg.buildcfg}"

    files {
        "vxltest/src/**.c"
    }

    includedirs {
        "vxl/include"
    }

    links {
        "vxl"
    }

    filter "system:macosx"
        includedirs {
            "/opt/homebrew/include"
        }

        libdirs {
            "/opt/homebrew/lib"
        }

        links {
            "SDL3"
        }

    filter "system:linux"
        links {
            "SDL3"
        }

    filter "system:windows"
        systemversion "latest"
        links {
            "SDL3"
        }

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"
