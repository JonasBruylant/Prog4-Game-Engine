#include "BurgerPlateComponent.h"
#include "CollisionComponent.h"
#include "BurgerPieceComponent.h"

dae::BurgerPlateComponent::BurgerPlateComponent(GameObject* pOwner):
	Component(pOwner)
{
}

void dae::BurgerPlateComponent::OnBurgerPieceHit(CollisionComponent* otherColComp, GameObject* otherGOCol)
{
	if (otherColComp->GetTag() == "MainBurgerPiece" && !otherGOCol->GetComponent<BurgerPieceComponent>()->GetIsFinished())
	{
		otherGOCol->GetComponent<BurgerPieceComponent>()->SetIsFinished(true);

		std::cout << otherGOCol->GetComponent<BurgerPieceComponent>()->GetIsFinished() << std::endl;
	}
}
