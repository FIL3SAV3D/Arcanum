project "ArcanumEditor"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++20"
	targetdir "Binaries/%{cfg.buildcfg}"
	staticruntime "off"

    files { "Source/**.h", "Source/**.cpp" }

    includedirs
    {
        "Source"
    }

    targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")