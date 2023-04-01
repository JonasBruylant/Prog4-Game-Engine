#pragma once
#include <map>
#include <memory>
#include <vector>
#include <utility>

#include "Singleton.h"
#include "ControllerInput.h"
#include "Command.h"



namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
		//Controller index will be connected to all the possible button presses
		using ControllerKey = std::pair<unsigned int, ControllerInput::ControllerButton>;

		//Link that button/ControllerKey to a command.
		using ControllerCommandsMap = std::map<ControllerKey, std::unique_ptr<Command>>;
		ControllerCommandsMap m_controllerCommands{};
		
		//Link a command with a key input from the keyboard/mouse.
		using KeyboardCommandsMap = std::map<SDL_Scancode, std::unique_ptr<Command>>;
		KeyboardCommandsMap m_keyboardCommands{ };

		ControllerCommandsMap m_commands{};
		//List of controllers to take input from.
		std::vector<std::unique_ptr<ControllerInput>> m_controllers;

		//https://stackoverflow.com/questions/3741055/inputs-in-sdl-on-key-pressed
		//^ Sent to me by Aaron Frans. ^
		std::vector<bool> m_pressedKeys = std::vector<bool>(322,false);
		std::vector<bool> m_upKeys = std::vector<bool>(322, false);
		std::vector<bool> m_downKeys = std::vector<bool>(322, false);
	public:
		bool ProcessInput(float deltaTime);

		void ProcessControllerInput(float deltaTime);
		void ProcessKeyBoardInput(float deltaTime);

		template<typename T>
		T* AddControllerCommandToMap(unsigned int controllerIdx, ControllerInput::ControllerButton button, std::weak_ptr<GameObject> gameActor);
		template<typename T> T* AddKeyboardCommandToMap(SDL_Scancode keyboardButton, std::weak_ptr<GameObject> gameActor);
		void AddController();
	};

	template<typename T>
	inline T* InputManager::AddControllerCommandToMap(unsigned int controllerIdx, ControllerInput::ControllerButton button, std::weak_ptr<GameObject> gameActor)
	{
		//https://stackoverflow.com/questions/5084209/check-if-template-argument-is-inherited-from-class
		//Check if T inherits from Command so only commands can be added.
		static_assert(std::is_base_of<Command, T>::value && "T isn't inherited from Command.");

		while (controllerIdx >= m_controllers.size())
		{
			AddController();
		}

		ControllerKey currentControllerButton = std::make_pair(controllerIdx, button);
		std::unique_ptr<T> uniqueCommandPtr = std::make_unique<T>(gameActor);

		auto returnPtr = uniqueCommandPtr.get();
		m_controllerCommands.insert({ currentControllerButton, std::move(uniqueCommandPtr) });
	
		return returnPtr;
	}

	template<typename T>
	inline T* InputManager::AddKeyboardCommandToMap(SDL_Scancode keyboardButton, std::weak_ptr<GameObject> gameActor)
	{
		//https://stackoverflow.com/questions/5084209/check-if-template-argument-is-inherited-from-class
		//Check if T inherits from Command so only commands can be added.
		static_assert(std::is_base_of<Command, T>::value && "T isn't inherited from Command.");


		std::unique_ptr<T> uniqueCommandPtr = std::make_unique<T>(gameActor);
		auto returnPtr = uniqueCommandPtr.get();

		m_keyboardCommands.insert({ keyboardButton , std::move(uniqueCommandPtr) });

		return returnPtr;
	}

}
