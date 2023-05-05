#include "TutorialComponent.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_plot.h"

#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl2.h>

dae::TutorialComponent::TutorialComponent(std::weak_ptr<GameObject> pOwner):
	Component(pOwner)
{
}

void dae::TutorialComponent::RenderImGUI()
{
	ImGui::Begin("Control tutorial");

	ImGui::Text("Player 1 Controls: ");
	ImGui::Text("keyboard X or Controller A button to kill.");
	ImGui::Text("keyboard Z or Controller B button to gain 100 score.");

	ImGui::Text("");

	ImGui::Text("Player 2 Controls: ");
	ImGui::Text("keyboard Q or Controller X button to kill.");
	ImGui::Text("keyboard E or Controller Y button to gain 100 score.");
	ImGui::End();
}
