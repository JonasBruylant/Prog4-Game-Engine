#pragma once
#include "Component.h"
#include "Observer.h"
#include <vector>
namespace dae
{
	class ScoreUIComponent;

class CheckLevelFinishedComponent final : public Component, public Observer
{
public:
	CheckLevelFinishedComponent(GameObject* pOwner);

	~CheckLevelFinishedComponent() = default;
	CheckLevelFinishedComponent(const CheckLevelFinishedComponent& other) = delete;
	CheckLevelFinishedComponent(CheckLevelFinishedComponent&& other) = delete;
	CheckLevelFinishedComponent& operator=(const CheckLevelFinishedComponent& other) = delete;
	CheckLevelFinishedComponent& operator=(CheckLevelFinishedComponent&& other) = delete;

	void Notify(Event event) override;
	void GoToNextScene();
	void Update() override;
	void IncrementUncompletedPieces();

	void AddScoreUIComponent(ScoreUIComponent* pScoreUIComponent);
private:
	int m_uncompletedPiecesAmount{};
	std::vector<ScoreUIComponent*> m_pPlayerUIScoreComps;
};
}

