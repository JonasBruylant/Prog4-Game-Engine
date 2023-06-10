#pragma once
#include "Component.h"
#include "Texture2D.h"
#include <glm/glm.hpp>
#include "Timer.h"
namespace dae {
	class TransformComponent;
	//class ImageObjectComponent;

class BurgerPieceComponent final : public Component
{
public:
	BurgerPieceComponent(GameObject* pOwner, const std::string& filePath);

	~BurgerPieceComponent();
	BurgerPieceComponent(const BurgerPieceComponent& other) = delete;
	BurgerPieceComponent(BurgerPieceComponent&& other) = delete;
	BurgerPieceComponent& operator=(const BurgerPieceComponent& other) = delete;
	BurgerPieceComponent& operator=(BurgerPieceComponent&& other) = delete;

	void Update() override;

	bool GetIsFalling() const { return m_IsFalling; }
	void SetIsFalling(bool falling) { m_IsFalling = falling; }

	void SetPlayer(GameObject* player) { m_pPlayer = player; }
	void SetIsFinished(bool isFinished) { m_IsFinished = isFinished; }

	std::vector<std::pair<bool, GameObject*>>& GetBurgerDivisions() { return m_BurgerDivisions; }
	std::shared_ptr<Texture2D> GetTexture() { return m_pBurgerPieceTexture; }

	void PushChildrenDown();
private:

	std::shared_ptr<Texture2D> m_pBurgerPieceTexture;
	TransformComponent* m_pOwnerTransform{ nullptr };
	static const int m_nrOfSubdivision{ 4 };
	std::vector<std::pair<bool, GameObject*>> m_BurgerDivisions;
	glm::vec3 m_pOwnerWorldPos{};

	GameObject* m_pPlayer{ nullptr };

	int m_srcRectWidth{};

	bool m_IsFinished{ false };

	float m_FallSpeed{ 40.f };
	bool m_IsFalling{ false };
	float m_FallPreventionTimer{ 0.f };
	float m_LastFallTime{};
	Timer& m_Timer = Timer::GetInstance();
};

}
