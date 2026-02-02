

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