#pragma once
#include <utility>
#include <map>
#include <memory>
#include <vector>

#include "Singleton.h"
#include "ControllerInput.h"
#include "Command.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
		//Controller index will be connected to all the possible button presses
		using ControllerKey = std::pair<unsigned int, ControllerInput::ControllerButton>;
		//Link that ControllerKey to a command.
		using ControllerCommandsMap = std::map<ControllerKey, std::unique_ptr<Command>>;
		

		ControllerCommandsMap m_commands{};
		//List of controllers to take input from.
		std::vector<std::unique_ptr<ControllerInput>> m_controllers;

	public:
		bool ProcessInput();

		unsigned int AddController();

		template <typename T>
		T* AddCommand(unsigned int controllerIdx, GameObject* actor, ControllerInput::ControllerButton input);
	};

	template<typename T>
	inline T* InputManager::AddCommand(unsigned int controllerIdx, GameObject* actor, ControllerInput::ControllerButton input)
	{
		while (controllerIdx >= m_controllers.size())
		{
			AddController();
		}

	}

}
