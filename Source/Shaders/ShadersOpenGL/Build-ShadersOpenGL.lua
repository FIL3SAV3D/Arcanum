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

    targetdir (BuildDir .. OutputDir )
    objdir (BuildDir .. "Intermediates/" .. OutputDir .. "/%{prj.name}")