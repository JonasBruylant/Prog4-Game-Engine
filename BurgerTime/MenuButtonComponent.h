#pragma once
#include "Component.h"
#include <functional>

namespace dae {
	class TextObjectComponent;
	class MenuButtonComponent final : public Component
	{
		
	public:
		MenuButtonComponent(GameObject* pOwner, std::function<void()> func, const std::string& buttonText, int buttonIdx);

		~MenuButtonComponent() = default;
		MenuButtonComponent(const MenuButtonComponent& other) = delete;
		MenuButtonComponent(MenuButtonComponent&& other) = delete;
		MenuButtonComponent& operator=(const MenuButtonComponent& other) = delete;
		MenuButtonComponent& operator=(MenuButtonComponent&& other) = delete;

		void Update() override;

		int GetIndex() { return m_buttonIndex; }

		void SetSelected(bool isActive) { m_isSelected = isActive; }
		bool GetIsSelected() { return m_isSelected; }

		void GetClicked();
	private:
		int m_buttonIndex{0};
		
		bool m_isSelected{ false };
		std::function<void()> m_func;
		TextObjectComponent* m_pOwnerTxtObjComp;
	};
}


