#include <SDL.h>
#include "InputManager.h"

#include "imgui.h"
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl2.h>

bool dae::InputManager::ProcessInput(float deltaTime)
{
	
	SDL_Event e;
	while (SDL_PollEvent(&e)) {

		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {

			m_pressedKeys[SDL_GetScancodeFromKey(e.key.keysym.sym)] = true;
			m_upKeys[SDL_GetScancodeFromKey(e.key.keysym.sym)] = true;
		}
		if (e.type == SDL_KEYUP) {

			m_pressedKeys[SDL_GetScancodeFromKey(e.key.keysym.sym)] = false;
			m_upKeys[SDL_GetScancodeFromKey(e.key.keysym.sym)] = true;
		}
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{

		}
		// etc...
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	//Source: ChatGPT.
	std::fill(m_upKeys.begin(), m_upKeys.end(), false);
	std::fill(m_downKeys.begin(), m_downKeys.end(), false);

	ProcessControllerInput(deltaTime);
	ProcessKeyBoardInput(deltaTime);

	return true;
}

void dae::InputManager::ProcessControllerInput(float deltaTime)
{
	for (auto& controller : m_controllers)
	{
		controller->Update();
	}

	for (auto& command : m_controllerCommands)
	{
		//Individual elements of the ControllerKey (pair) variable
		const unsigned int& index = command.first.first;
		const ControllerInput::ControllerButton& button = command.first.second;

		//Command is unique pointer, so it can't be copied, take a reference to the adress instead.
		const auto& actualCommand = command.second.get();
		auto commandButtonState = actualCommand->GetBoundButtonState();


		if (commandButtonState == Command::Command::BoundButtonState::Down)
		{
			if (m_controllers[index]->IsDown(button))
				actualCommand->Execute(deltaTime);

		}
		else if (commandButtonState == Command::Command::BoundButtonState::Up)
		{
			if (m_controllers[index]->IsUp(button))
				actualCommand->Execute(deltaTime);

		}
		else if (commandButtonState == Command::Command::BoundButtonState::Pressed)
		{
			if (m_controllers[index]->IsPressed(button))
				actualCommand->Execute(deltaTime);

		}
	}

}


void dae::InputManager::ProcessKeyBoardInput(float deltaTime)
{
	for (auto& command : m_keyboardCommands)
	{
		//Individual elements of the ControllerKey (pair) variable		
		const unsigned& keyboardInput = command.first;


		//Command is unique pointer, so it can't be copied, take a reference to the adress instead.
		const auto& actualCommand = command.second.get();
		auto commandButtonState = actualCommand->GetBoundButtonState();

		if (commandButtonState == Command::Command::BoundButtonState::Down)
		{
			if (m_downKeys[keyboardInput])
				actualCommand->Execute(deltaTime);
		}
		else if (commandButtonState == Command::Command::BoundButtonState::Up)
		{
			if (m_upKeys[keyboardInput])
				actualCommand->Execute(deltaTime);
		}
		else if (commandButtonState == Command::Command::BoundButtonState::Pressed)
		{
			if (m_pressedKeys[keyboardInput])
				actualCommand->Execute(deltaTime);
		}

	}
}

void dae::InputManager::AddController()
{
	unsigned int controllerIndex = static_cast<unsigned int>(m_controllers.size());
	m_controllers.push_back(std::make_unique<ControllerInput>(controllerIndex));
}
