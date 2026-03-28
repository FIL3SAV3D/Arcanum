#include "UIRenderSystem.h"

#include "ArcEngine/Platform/Window.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

void UIRenderSystem::GetSignature(SignatureParameters& _Parameters)
{

}

void UIRenderSystem::OnRenderUI(RenderState& _RenderState)
{
    //ImGui::ShowDemoWindow();

    if(ImGui::Begin("Project Window", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse))
    {
        ImGui::Text("Connect to host IP Adress:");
        ImGui::InputText("IP Adress", &adress);
        if (ImGui::Button("Connect"))
        {
            adress.clear();
        }
        ImGui::Dummy(ImVec2(0.0f, 2.0f));
        ImGui::Text("Host Temp Project");
        if (ImGui::Button("Host"))
        {
            adress.clear();
        }

        ImGui::End();
    }
}