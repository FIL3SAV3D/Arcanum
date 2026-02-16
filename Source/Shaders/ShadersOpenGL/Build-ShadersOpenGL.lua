project "ShadersOpenGL"
    kind "Utility"
	targetdir "Binaries/%{cfg.buildcfg}"

    files 
    { 
        "Source/**.frag",
        "Source/**.vert",
        "Source/**.comp"
    }

    includedirs
    {
        "Source",
    }

    targetdir ("../../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")