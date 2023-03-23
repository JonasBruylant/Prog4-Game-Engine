#pragma once
#include "Singleton.h"
#include <utility>
#include "ControllerInput.h"
#include <map>
#include <memory>
#include <vector>
namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
		//Assign a number to every button.
		using ControllerKey = std::pair<unsigned int, ControllerInput::ControllerButton>;

		//Link that button/ControllerKey to a command.
		using ControllerCommandsMap = std::map<ControllerKey, std::unique_ptr</*Command*/>>;
		
		ControllerCommandsMap m_commands{};

		//List of controllers to take input from.
		std::vector<std::unique_ptr<ControllerInput>> m_controllers;

	public:
		bool ProcessInput();
	};

}
