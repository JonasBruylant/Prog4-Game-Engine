#pragma once
#include "Component.h"
#include "Texture2D.h"

namespace dae {
	class TransformComponent;
	//class ImageObjectComponent;

class BurgerPieceComponent : public Component
{
public:
	BurgerPieceComponent(GameObject* pOwner, const std::string& filePath);

	~BurgerPieceComponent();
	BurgerPieceComponent(const BurgerPieceComponent& other) = delete;
	BurgerPieceComponent(BurgerPieceComponent&& other) = delete;
	BurgerPieceComponent& operator=(const BurgerPieceComponent& other) = delete;
	BurgerPieceComponent& operator=(BurgerPieceComponent&& other) = delete;

	void Update() override;

private:

	std::shared_ptr<Texture2D> m_pBurgerPieceTexture;
	TransformComponent* m_pOwnerTransform{ nullptr };
	static const int m_nrOfSubdivision{ 4 };
	std::vector<std::pair<bool, GameObject*>> m_BurgerDivisions;


	float m_FallSpeed{ 40.f };
};

}
