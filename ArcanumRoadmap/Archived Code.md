

mode manager
	// Application Being Created
	void OnCreate();
	void OnStart();
	void OnEnable();

	// Update
	void OnInput();
	void OnUpdate(const float& _DeltaTime);
	void OnLateUpdate(const float& _DeltaTime);
	void OnRender();
	void OnRenderUI();
	void OnApplicationPause();
	void OnCheckForDisabled();

	// End
	void OnDisable();
	void OnQuit();
	void OnDestroy();


Shader compilation:
´´´c++
project "Shaders"  
        kind "Utility"  
        targetdir "bin/%{cfg.buildcfg}"  
  
        files{  
                "source/Shaders/**.frag",  
                "source/Shaders/**.vert"  
        }  
  
        filter 'files:**.frag'  
            buildmessage 'Compiling %{file.relpath}'  
  
            buildcommands{ "%{vulkan_sdk}/Bin/glslc.exe %{file.abspath} -o %{wks.location}source/shaders/compiled_shaders/%{file.basename}.frag.spv" }  
  
            buildoutputs { "source/shaders/compiled_shaders/%{file.basename}.frag.spv" }  
  
        filter 'files:**.vert'  
            buildmessage 'Compiling %{file.relpath}'  
  
            buildcommands{ "%{vulkan_sdk}/Bin/glslc.exe %{file.abspath} -o %{wks.location}source/shaders/compiled_shaders/%{file.basename}.vert.spv" }  
  
            buildoutputs { "source/shaders/compiled_shaders/%{file.basename}.vert.spv" }
´´´