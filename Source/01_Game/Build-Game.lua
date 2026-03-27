project "Game"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
	targetdir "Binaries/%{cfg.buildcfg}"
	staticruntime (StaticLinking)

    files { "Source/**.h", "Source/**.cpp" }

    includedirs
    {
        "Source",
        "../02_Framework/Source",

        "../../Library/GLM",
        "../../Library/fmt/include",
    }

    links
    {
        "Framework",
    }

    targetdir (BuildDir .. OutputDir )
    objdir (BuildDir .. "Intermediates/" .. OutputDir .. "/%{prj.name}")

    filter "system:windows"
        systemversion "latest"
        defines { }

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

        links 
        { 
         "../../Library/fmt/lib/fmtd.lib",
        }

    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "On"

        links 
        { 
        "../../Library/fmt/lib/fmt.lib",
        }

    filter "configurations:Dist"
        defines { "DIST" }
        runtime "Release"
        optimize "On"
        symbols "Off"

        links 
        { 
        "../../Library/fmt/lib/fmt.lib",
        }