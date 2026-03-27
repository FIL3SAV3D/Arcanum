project "Arcanum"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
	targetdir "Binaries/%{cfg.buildcfg}"
	staticruntime (StaticLinking)

    files { "Source/**.h", "Source/**.cpp" }

    includedirs
    {
        "Source",
        "../01_Editor/Source"
    }

    links
    {
        "Editor"
    }

    -- targetdir ("../../Binaries/" .. OutputDir .. "/%{prj.name}")
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
        }

    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "On"

        links 
        { 
        }

    filter "configurations:Dist"
        defines { "DIST" }
        runtime "Release"
        optimize "On"
        symbols "Off"

        links 
        { 
        }