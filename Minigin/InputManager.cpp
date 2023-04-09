#include <SDL.h>
#include "InputManager.h"

#include "imgui.h"
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl2.h>

bool dae::InputManager::ProcessInput()
{
	m_previousState = m_currentState;


	SDL_Event e;
	while (SDL_PollEvent(&e)) {

		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			m_currentState[SDL_GetScancodeFromKey(e.key.keysym.sym)] = true;
		}
		if (e.type == SDL_KEYUP) {
			m_currentState[SDL_GetScancodeFromKey(e.key.keysym.sym)] = false;
		}
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{

		}
		// etc...
		ImGui_ImplSDL2_ProcessEvent(&e);

	}


	ProcessControllerInput();
	ProcessKeyBoardInput();

	return true;
}

void dae::InputManager::ProcessControllerInput()
{
	for (auto& controller : m_controllers)
	{
		controller->Update();
	}

	for (auto& command : m_controllerCommands)
	{
		//Individual elements of the ControllerKey (pair) variable
		const unsigned int& index = command.first.controllerIdx;
		const Controller::ControllerButton& button = command.first.button;
		const ButtonState buttonState = command.first.buttonState;
		//Command is unique pointer, so it can't be copied, take a reference to the adress instead.
		const auto& actualCommand = command.second.get();
		
		switch (buttonState)
		{
			case ButtonState::Down:
			{
				if (m_controllers[index]->IsDown(button))
					actualCommand->Execute();
				break;
			}
			case ButtonState::Up:
			{
				if (m_controllers[index]->IsUp(button))
					actualCommand->Execute();
				break;
			}			
			case ButtonState::Pressed:
			{
				if (m_controllers[index]->IsPressed(button))
					actualCommand->Execute();

				break;
			}
		}

	}

}


void dae::InputManager::ProcessKeyBoardInput()
{
	for (auto& command : m_keyboardCommands)
	{

		const unsigned& keyboardInput = command.first.keyboardButton;
		const ButtonState keyboardButtonState = command.first.buttonState;


		//Command is unique pointer, so it can't be copied, take a reference to the adress instead.
		const auto& actualCommand = command.second.get();

		switch (keyboardButtonState)
		{
			case ButtonState::Down:
			{
				if (!m_previousState[keyboardInput] && m_currentState[keyboardInput])
					actualCommand->Execute();
				break;
			}
			case ButtonState::Up:
			{
				if (m_previousState[keyboardInput] && !m_currentState[keyboardInput])
					actualCommand->Execute();
				break;
			}
			case ButtonState::Pressed:
			{
				if (m_previousState[keyboardInput] && m_currentState[keyboardInput])
					actualCommand->Execute();

				break;
			}
		}



	}
}

void dae::InputManager::AddController()
{
	unsigned int controllerIndex = static_cast<unsigned int>(m_controllers.size());
	m_controllers.push_back(std::make_unique<Controller>(controllerIndex));
}
