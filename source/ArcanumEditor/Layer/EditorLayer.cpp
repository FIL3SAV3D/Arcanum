#include "EditorLayer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>




EditorLayer::EditorLayer(std::shared_ptr<Window> _Window):
	ILayer{"Editor", DEFAULT_PRIORITIES::EDITOR},
	window{_Window}
{
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnAttach()
{
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(const float& _DeltaTime)
{
	
}

void EditorLayer::OnImGuiRender()
{

}
