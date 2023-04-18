#pragma once
#include "Component.h"

namespace dae {

class TutorialComponent final : public Component
{
public:
	TutorialComponent(std::weak_ptr<GameObject> pOwner);
	virtual ~TutorialComponent() = default;
	TutorialComponent(const TutorialComponent& other) = delete;
	TutorialComponent(TutorialComponent&& other) = delete;
	TutorialComponent& operator=(const TutorialComponent& other) = delete;
	TutorialComponent& operator=(TutorialComponent&& other) = delete;

	void RenderImGUI() override;

};
}

