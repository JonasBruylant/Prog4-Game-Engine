#include "MenuButtonComponent.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "TextObjectComponent.h"

dae::MenuButtonComponent::MenuButtonComponent(GameObject* pOwner, std::function<void()> func, const std::string& buttonText, int buttonIdx) :
	Component(pOwner), m_func{func}, m_buttonIndex{buttonIdx}
{
	m_pOwnerTxtObjComp = pOwner->GetComponent<TextObjectComponent>().get();
	m_pOwnerTxtObjComp->SetText(buttonText);
	
}

void dae::MenuButtonComponent::Update()
{
	if (m_isSelected)
		m_pOwnerTxtObjComp->SetColor(255, 255, 0);
	else
		m_pOwnerTxtObjComp->SetColor(255, 255, 255);
}

void dae::MenuButtonComponent::GetClicked() 
{ 
	m_func();
}