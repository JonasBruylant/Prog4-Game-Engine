#include <SDL.h>
#include "InputManager.h"

#include "imgui.h"
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl2.h>

bool dae::InputManager::ProcessInput()
{
	
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {

		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {

		}
		// etc...'


		//Process imput for IMGUI
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	return true;
}


/// <summary>
/// This method adds a controller to the list for the inputManager to check up on.
/// </summary>
/// <returns>The index of the controller added.</returns>
unsigned int dae::InputManager::AddController()
{
	//Adds a controller to the list with the index of the current list size.
	std::unique_ptr currentController = std::make_unique<ControllerInput>(static_cast<unsigned int>(m_controllers.size()));
	m_controllers.push_back(currentController);

	return static_cast<unsigned int>(m_controllers.size()) - 1;
}
