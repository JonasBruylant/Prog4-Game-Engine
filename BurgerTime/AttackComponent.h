#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae
{
	class Scene;
	class StateComponent;
class AttackComponent final : public Component
{
public:
	AttackComponent(GameObject* pOwner);

	~AttackComponent() = default;
	AttackComponent(const AttackComponent& other) = delete;
	AttackComponent(AttackComponent&& other) = delete;
	AttackComponent& operator=(const AttackComponent& other) = delete;
	AttackComponent& operator=(AttackComponent&& other) = delete;

	void Attack();
	Subject* GetAttackSubject() { return m_pPepperAttackUsedEvent.get(); }
private:
	void SpawnPepper();

	StateComponent* m_pStateComponent;
	Scene* m_pCurrentScene;
	std::unique_ptr<Subject> m_pPepperAttackUsedEvent = std::make_unique<Subject>();
	std::string m_filePath{};

	int m_attacksLeft{ 5 };
};

}
