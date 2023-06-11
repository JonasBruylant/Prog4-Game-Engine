#include "BurgerPieceComponent.h"
#include "ResourceManager.h"

#include "ImageObjectComponent.h"
#include "ImageRenderComponent.h"
#include "CollisionComponent.h"
#include "ScoreComponent.h"
#include "EnemyComponent.h"

#include "Timer.h"

dae::BurgerPieceComponent::BurgerPieceComponent(GameObject* pOwner, const std::string& filePath):
	Component(pOwner)
{
	m_FilePath = filePath;
	m_pBurgerPieceTexture = ResourceManager::GetInstance().LoadTexture(filePath);
	m_srcRectWidth = m_pBurgerPieceTexture->GetSize().x / m_nrOfSubdivision;
	auto textureHeight = m_pBurgerPieceTexture->GetSize().y;

	m_pOwnerTransform = pOwner->GetTransform();

	auto& ownerWorldPos = m_pOwnerTransform->GetWorldPosition();
	for (int i = 0; i < m_nrOfSubdivision; ++i)
	{
		std::shared_ptr<GameObject> burgerPieceChild = std::make_shared<GameObject>();
		burgerPieceChild->Initialize();
		burgerPieceChild->AddComponent<dae::ImageObjectComponent>()->SetTexture(m_pBurgerPieceTexture);

		auto imageRenderComp = burgerPieceChild->AddComponent<dae::ImageRenderComponent>();
		imageRenderComp->SetSrcRect({ i * m_srcRectWidth, 0, m_srcRectWidth, textureHeight });
		imageRenderComp->SetDestRect({ static_cast<int>(ownerWorldPos.x + (i * m_srcRectWidth)), static_cast<int>(ownerWorldPos.y), m_srcRectWidth, textureHeight});
		imageRenderComp->SetRenderUsingRects(true);



		burgerPieceChild->SetScene(pOwner->GetScene());

		auto childColComp = burgerPieceChild->AddComponent<CollisionComponent>();
		childColComp->SetTag("BurgerPiece");
		childColComp->SetMeasurements(static_cast<float>(m_srcRectWidth),static_cast<float>(textureHeight));
		childColComp->SetDebugColor({rand() % 256,rand() % 256 ,rand() % 256 });
		//childColComp->SetDrawDebug(true);
		childColComp->SetOnTriggerEvent([=](CollisionComponent* otherColComp, GameObject* otherGOCol)
			{
				if (otherColComp->GetTag() == "Player" || otherColComp->GetTag() == "PlayerLevelCheck")
				{
					m_BurgerDivisions[i].first = true;
					auto burgerChildTransform = m_BurgerDivisions[i].second->GetTransform();
					//auto burgerChildWorldPos = burgerChildTransform->GetWorldPosition();
					burgerChildTransform->SetLocalPosition(static_cast<float>(i * m_srcRectWidth), 7, 0);
					return;
				}
				
				if (otherColComp->GetTag() == "Enemy")
				{
					if (m_IsFalling)
					{
						if (!otherGOCol->GetMarkedForDelete())
						{
							otherGOCol->SetMarkedForDelete();
							m_pPlayer->GetComponent<dae::ScoreComponent>()->GainScore(otherGOCol->GetComponent<EnemyComponent>()->GetEnemyType());
						}

					}
				}

				if (otherColComp->GetTag() == "LevelPlatform")
				{
					if (m_IsFalling)
						m_IsFalling = false;
				}
			});																					  
																								  
																								  
																								  
		burgerPieceChild->SetParent(pOwner, true);
		burgerPieceChild->GetTransform()->SetLocalPosition(static_cast<float>(i * m_srcRectWidth),0,0);


		m_BurgerDivisions.emplace_back(std::make_pair(false, burgerPieceChild.get()));
	}


	auto goCollisionComponent = pOwner->AddComponent<CollisionComponent>();
	goCollisionComponent->SetTag("MainBurgerPiece");
	goCollisionComponent->SetMeasurements(static_cast<float>(m_pBurgerPieceTexture->GetSize().x), static_cast<float>(m_pBurgerPieceTexture->GetSize().y));
	//goCollisionComponent->SetDrawDebug(true);
	//auto goCollisionComponent = go->AddComponent<CollisionComponent>();
	goCollisionComponent->SetOnTriggerEvent([&](CollisionComponent* otherColComp, GameObject* otherGOCol)
		{
			if (m_IsFinished)
				return;


			if (otherColComp->GetTag() != "MainBurgerPiece")
				//std::cout << "BunTop!! Other tag is not MainBurgerPiece, it's: " + otherColComp->GetTag() << std::endl;
				return;

			//std::cout << "Other collision is another BurgerPiece\n";


			auto otherBurgerPiece = otherGOCol->GetComponent<BurgerPieceComponent>();
			if (!otherBurgerPiece)
				return;
			
			if (otherBurgerPiece->GetIndex() < m_burgerPieceIdx)
				return;

			if (otherBurgerPiece->GetIsFinished())
			{
				m_IsFinished = true;
				m_pFinishedEvent->NotifyObservers({ "Piece Finished" });
				return;
			}

			//std::cout << "Other collision has BurgerPieceComponent\n";

			otherBurgerPiece->PushChildrenDown();

		}
	);



}

void dae::BurgerPieceComponent::Update()
{
	
	bool hasAllBeenSteppedOn{ true };
	m_pOwnerWorldPos = m_pOwnerTransform->GetWorldPosition();
	for (int i = 0; i < m_nrOfSubdivision; ++i)
	{
		if (!m_BurgerDivisions[i].first)
		{
			hasAllBeenSteppedOn = false;
			break;
		}
	}
	//std::cout << m_FilePath + " " << m_IsFinished << std::endl;
	if (hasAllBeenSteppedOn && !m_IsFinished)
	{
		m_IsFalling = true;
		m_pOwnerTransform->SetLocalPosition({ m_pOwnerWorldPos.x, m_pOwnerWorldPos.y + 16,0 });
		for (int j = 0; j < m_BurgerDivisions.size(); ++j) //Iterate over children																					   
		{

			m_BurgerDivisions[j].first = false;
			//m_BurgerDivisions[j].second->GetTransform()->SetLocalPosition({ static_cast<int>(m_pOwnerWorldPos.x + (j * m_srcRectWidth)),5,0 });

		}
	}

	if (m_IsFalling && !m_IsFinished)
	{
		
		m_FallPreventionTimer += m_Timer.GetDeltaTime();
		//std::cout << m_FallPreventionTimer << std::endl;
		m_pOwnerTransform->AddToLocalPosition({0,m_FallSpeed * m_Timer.GetDeltaTime(),0});
	}

	//auto scene = GetOwner()->GetScene();
	//scene
}

void dae::BurgerPieceComponent::SetIsFinished(bool isFinished)
{
	m_IsFinished = isFinished;

	if(m_IsFinished)
		m_pFinishedEvent->NotifyObservers({ "Piece Finished" });
}

void dae::BurgerPieceComponent::PushChildrenDown()
{				
	if (m_IsFinished)
		return;


	if (m_IsFalling == true)
	{
		m_LastFallTime = m_Timer.GetTotalTime();
		m_IsFalling = false;
		m_FallPreventionTimer = 0.f;
		for (int j = 0; j < m_BurgerDivisions.size(); ++j) //Iterate over children																					   
		{

			m_BurgerDivisions[j].first = false;
			//m_BurgerDivisions[j].second->GetTransform()->SetLocalPosition({ static_cast<int>(m_pOwnerWorldPos.x + (j * m_srcRectWidth)),5,0 });

		}

		return;
	}

	if (m_LastFallTime + 5.f > m_Timer.GetTotalTime())
		return;


	m_pOwnerTransform->SetLocalPosition({ m_pOwnerWorldPos.x, m_pOwnerWorldPos.y + 16,0});
	for (int j = 0; j < m_BurgerDivisions.size(); ++j) //Iterate over children																					   
	{
		
		m_BurgerDivisions[j].first = true;
		//m_BurgerDivisions[j].second->GetTransform()->SetLocalPosition({ static_cast<int>(m_pOwnerWorldPos.x + (j * m_srcRectWidth)),5,0 });

	}
	m_IsFalling = true;
}
