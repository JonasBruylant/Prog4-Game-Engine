#include "ButtonIndexManagerComponent.h"
#include "MenuButtonComponent.h"


dae::ButtonIndexManagerComponent::ButtonIndexManagerComponent(GameObject* pOwner, std::vector<MenuButtonComponent*> buttons):
	Component(pOwner), m_menuButtons{buttons}
{
}

void dae::ButtonIndexManagerComponent::UpdateIndex(bool isNextButton)
{
	m_menuButtons[m_currentSelectedIndex]->SetSelected(false);

	if (isNextButton)
		++m_currentSelectedIndex;
	else
		--m_currentSelectedIndex;

	if (m_currentSelectedIndex > 2)
		m_currentSelectedIndex = 0;
	else if (m_currentSelectedIndex < 0)
		m_currentSelectedIndex = 2;

	m_menuButtons[m_currentSelectedIndex]->SetSelected(true);
}

void dae::ButtonIndexManagerComponent::ExecuteActiveButton()
{
	m_menuButtons[m_currentSelectedIndex]->GetClicked();
}
