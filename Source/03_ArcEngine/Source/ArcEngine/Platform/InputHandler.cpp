#include "InputHandler.h"

#include "iInputListener.h"

#include <GLFW/glfw3.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_oldnames.h>

InputHandler::InputHandler()
{
}

InputHandler::~InputHandler()
{
	size_t size = listeners.size();
	for (size_t i = 0; i < size; i++)
	{
		listeners.at(i).reset();
		RemoveListener(listeners.at(i));
	}
}

void InputHandler::ProcessInput()
{
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0) {
		// close the window when user alt-f4s or clicks the X button
		if (e.type == SDL_EVENT_QUIT)
			quit = true;

		//if (e.type == SDL_EVENT_WINDOW_RESIZED)
		//{
		//	int x{}, y{};
		//	SDL_GetWindowSize(window->GetNativeWindow(), &x, &y);
		//	graphics->Resize(glm::uvec2(x, y));
		//}

		const bool* key_states = SDL_GetKeyboardState(nullptr);

		for (auto listener : listeners)
		{
			listener->UpdateInput(key_states);
		}

		if (key_states[SDL_SCANCODE_ESCAPE])
		{
			quit = true;
		}

		//if (e.window.type == SDL_EVENT_WINDOW_MINIMIZED) {
		//	stop_rendering = true;
		//}
		//if (e.window.type == SDL_EVENT_WINDOW_RESTORED) {
		//	stop_rendering = false;
		//}

		//ImGui_ImplSDL3_ProcessEvent(&e);
	}

	//if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	//{
	//	glfwSetWindowShouldClose(window, true);
	//}

	//

	//if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	//{
	//	glfwSetWindowShouldClose(window, true);
	//}
}

void InputHandler::AddListener(const std::shared_ptr<iInputListener> _listener)
{
	listeners.push_back(_listener);
}

void InputHandler::RemoveListener(const std::shared_ptr<iInputListener> _listener)
{
	std::vector<std::shared_ptr<iInputListener>>::iterator itr = std::find(listeners.begin(), listeners.end(), _listener);
	listeners.erase(itr);
}
