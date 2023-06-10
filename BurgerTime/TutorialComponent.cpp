#include "TutorialComponent.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_plot.h"

#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl2.h>

dae::TutorialComponent::TutorialComponent(GameObject* pOwner):
	Component(pOwner)
{
}

void dae::TutorialComponent::RenderImGUI()
{
	ImGui::Begin("Control tutorial");

	ImGui::Text("Controls: ");
	ImGui::Text("WASD to move");
	ImGui::Text("X to gain score");

	ImGui::End();
}
