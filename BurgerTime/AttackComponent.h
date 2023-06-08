#pragma once
#include "Component.h"

namespace dae
{
	class Scene;
	class StateComponent;
class AttackComponent final : public Component
{
public:
	AttackComponent(std::weak_ptr<GameObject> pOwner);

	~AttackComponent() = default;
	AttackComponent(const AttackComponent& other) = delete;
	AttackComponent(AttackComponent&& other) = delete;
	AttackComponent& operator=(const AttackComponent& other) = delete;
	AttackComponent& operator=(AttackComponent&& other) = delete;

	void Attack();
private:
	void SpawnPepper();

	StateComponent* m_pStateComponent;
	Scene* m_pCurrentScene;

	std::string m_filePath{};
};

}
