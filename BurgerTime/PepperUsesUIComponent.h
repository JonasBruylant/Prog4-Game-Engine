#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae
{
	class TextObjectComponent;
	class AttackComponent;
class PepperUsesUIComponent final : public Component, public Observer
{
public:
	PepperUsesUIComponent(GameObject* pOwner);

	~PepperUsesUIComponent() = default;
	PepperUsesUIComponent(const PepperUsesUIComponent& other) = delete;
	PepperUsesUIComponent(PepperUsesUIComponent&& other) = delete;
	PepperUsesUIComponent& operator=(const PepperUsesUIComponent& other) = delete;
	PepperUsesUIComponent& operator=(PepperUsesUIComponent&& other) = delete;

	void Notify(Event event) override;
	void UpdatePepperUsage();
private:
	int m_amountOfPepperUsage{ 5 };
	TextObjectComponent* m_pTextObjectPepperComponent{ nullptr };
};
}

