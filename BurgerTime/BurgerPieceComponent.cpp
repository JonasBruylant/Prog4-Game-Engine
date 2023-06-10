#include "BurgerPieceComponent.h"
#include "ResourceManager.h"

#include "ImageObjectComponent.h"
#include "ImageRenderComponent.h"
#include "CollisionComponent.h"

#include "Timer.h"

dae::BurgerPieceComponent::BurgerPieceComponent(GameObject* pOwner, const std::string& filePath):
	Component(pOwner)
{
	m_pBurgerPieceTexture = ResourceManager::GetInstance().LoadTexture(filePath);
	auto textureWidth = m_pBurgerPieceTexture->GetSize().x / m_nrOfSubdivision;
	auto textureHeight = m_pBurgerPieceTexture->GetSize().y;

	auto& ownerWorldPos = pOwner->GetTransform()->GetWorldPosition();
	for (int i = 0; i < m_nrOfSubdivision; ++i)
	{
		std::shared_ptr<GameObject> burgerPieceChild = std::make_shared<GameObject>();
		burgerPieceChild->Initialize();
		burgerPieceChild->AddComponent<dae::ImageObjectComponent>()->SetTexture(m_pBurgerPieceTexture);
		burgerPieceChild->AddComponent<dae::ImageRenderComponent>()->SetSrcRect({ i * textureWidth, 0, textureWidth, textureHeight });
		burgerPieceChild->AddComponent<dae::ImageRenderComponent>()->SetDestRect({ static_cast<int>(ownerWorldPos.x), static_cast<int>(ownerWorldPos.y), textureWidth, textureHeight});



		burgerPieceChild->SetScene(pOwner->GetScene());

		auto colComp = burgerPieceChild->AddComponent<CollisionComponent>();
		colComp->SetTag("BurgerPiece");
		colComp->SetMeasurements(static_cast<float>(textureWidth),static_cast<float>(textureHeight));
		colComp->SetDebugColor({rand() % 256,rand() % 256 ,rand() % 256 });
		colComp->SetOnTriggerEvent([=](CollisionComponent* otherColComp, GameObject*)
			{
				if (otherColComp->GetTag() == "Player")
				{
					m_BurgerDivisions[i].first = true;
					auto burgerChildTransform = m_BurgerDivisions[i].second->GetTransform();
					burgerChildTransform->SetLocalPosition(0, 2, 0);
				}

				if (otherColComp->GetTag() == "BurgerPiece")
				{
					for (int j = 0; j < m_BurgerDivisions.size(); ++j)
					{
						m_BurgerDivisions[j].first = false;
					}
				}

			});



		burgerPieceChild->SetParent(pOwner, true);
		burgerPieceChild->GetTransform()->SetLocalPosition(static_cast<float>(i * textureWidth),0,0);


		m_BurgerDivisions.emplace_back(std::make_pair(false, burgerPieceChild.get()));
	}



}

dae::BurgerPieceComponent::~BurgerPieceComponent()
{
	//for (int i = 0; i < m_BurgerDivisions.size(); ++i)
	//{
	//	delete m_BurgerDivisions[i].second;
	//	m_BurgerDivisions[i].second = nullptr;
	//}
}

void dae::BurgerPieceComponent::Update()
{
	bool hasAllBeenSteppedOn{ true };

	for (int i = 0; i < m_nrOfSubdivision; ++i)
	{
		if (!m_BurgerDivisions[i].first)
		{
			hasAllBeenSteppedOn = false;
			break;
		}
	}

	if (hasAllBeenSteppedOn)
	{
		GetOwner()->GetTransform()->AddToLocalPosition({0,m_FallSpeed * Timer::GetInstance().GetDeltaTime(),0});
	}
}
