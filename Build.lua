workspace "Arcanum"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "ArcanumEditor"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "ArcanumEditor"
	include "source/ArcanumEditor/Build-ArcanumEditor.lua"
group ""

--include "App/Build-App.lua"