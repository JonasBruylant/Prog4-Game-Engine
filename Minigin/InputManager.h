#pragma once
#include <map>
#include <memory>
#include <vector>
#include <utility>

#include "Singleton.h"
#include "Controller.h"
#include "Command.h"



namespace dae
{
	enum ButtonState {
		Up,
		Down,
		Pressed
	};

	struct ControllerInput
	{
		unsigned int controllerIdx;
		Controller::ControllerButton button;
		ButtonState buttonState;

		bool operator<(const ControllerInput& other) const
		{
			if (controllerIdx < other.controllerIdx)
				return true;
			else if (controllerIdx > other.controllerIdx)
				return false;

			if (button < other.button)
				return true;
			else if (button > other.button)
				return false;

			if (buttonState < other.buttonState)
				return true;
			else
				return false;

		}
	};

	struct KeyBoardInput
	{
		SDL_Scancode keyboardButton;
		ButtonState buttonState;

		bool operator<(const KeyBoardInput& other) const
		{
			if (keyboardButton < other.keyboardButton)
				return true;
			else if (keyboardButton > other.keyboardButton)
				return false;

			if (buttonState < other.buttonState)
				return true;
			else
				return false;

		}
	};

	class GameObject;

	class InputManager final : public Singleton<InputManager>
	{

		//Link that button/ControllerKey to a command.
		using ControllerCommandsMap = std::map<ControllerInput, std::unique_ptr<Command>>;
		ControllerCommandsMap m_controllerCommands{};

		//List of controllers to take input from.
		std::vector<std::unique_ptr<Controller>> m_controllers;

		void ProcessControllerInput();


		//Link a command with a key input from the keyboard/mouse.
		using KeyboardCommandsMap = std::map<KeyBoardInput, std::unique_ptr<Command>>;
		KeyboardCommandsMap m_keyboardCommands{ };

		//https://stackoverflow.com/questions/3741055/inputs-in-sdl-on-key-pressed
		//^ Sent to me by Aaron Frans. ^

		std::vector<bool> m_currentState= std::vector<bool>(322, false);
		std::vector<bool> m_previousState= std::vector<bool>(322, false);



	public:
		bool ProcessInput();
		void ProcessKeyBoardInput();

		template<typename T> T* AddControllerCommandToMap(ControllerInput controllerInput, std::unique_ptr<T> command);
		template<typename T> T* AddKeyboardCommandToMap(KeyBoardInput keyboardInput, std::unique_ptr<T> command);
		void AddController();
	};

	template<typename T>
	inline T* InputManager::AddControllerCommandToMap(ControllerInput controllerInput, std::unique_ptr<T> command)
	{
		//https://stackoverflow.com/questions/5084209/check-if-template-argument-is-inherited-from-class
		//Check if T inherits from Command so only commands can be added.
		static_assert(std::is_base_of<Command, T>::value && "T isn't inherited from Command.");

		while (controllerInput.controllerIdx >= m_controllers.size())
		{
			AddController();
		}

		auto returnPtr = command.get();
		m_controllerCommands.insert({ controllerInput, std::move(command) });

		return returnPtr;
	}

	template<typename T>
	inline T* InputManager::AddKeyboardCommandToMap(KeyBoardInput keyboardInput, std::unique_ptr<T> command)
	{
		//https://stackoverflow.com/questions/5084209/check-if-template-argument-is-inherited-from-class
		//Check if T inherits from Command so only commands can be added.
		static_assert(std::is_base_of<Command, T>::value && "T isn't inherited from Command.");

		auto returnPtr = command.get();
		m_keyboardCommands.insert({ keyboardInput, std::move(command) });

		return returnPtr;
	}

}
