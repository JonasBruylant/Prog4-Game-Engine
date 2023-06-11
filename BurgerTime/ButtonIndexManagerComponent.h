#pragma once
#include "Component.h"
#include <vector>


namespace dae
{
	class MenuButtonComponent;
	class ButtonIndexManagerComponent final : public Component
	{
	public:
		ButtonIndexManagerComponent(GameObject* pOwner, std::vector<MenuButtonComponent*> buttons);

		~ButtonIndexManagerComponent() = default;
		ButtonIndexManagerComponent(const ButtonIndexManagerComponent& other) = delete;
		ButtonIndexManagerComponent(ButtonIndexManagerComponent&& other) = delete;
		ButtonIndexManagerComponent& operator=(const ButtonIndexManagerComponent& other) = delete;
		ButtonIndexManagerComponent& operator=(ButtonIndexManagerComponent&& other) = delete;

		void UpdateIndex(bool isNextButton);
		void ExecuteActiveButton();


	private:
		int m_currentSelectedIndex{ 0 };
		std::vector<MenuButtonComponent*> m_menuButtons;
	};

}
