#pragma once
#include "Component.h"

namespace dae {

	class CollisionComponent;
	class BurgerPlateComponent final : public Component
	{
	
	public:
		BurgerPlateComponent(GameObject* pOwner);
	
		~BurgerPlateComponent() = default;
		BurgerPlateComponent(const BurgerPlateComponent& other) = delete;
		BurgerPlateComponent(BurgerPlateComponent&& other) = delete;
		BurgerPlateComponent& operator=(const BurgerPlateComponent& other) = delete;
		BurgerPlateComponent& operator=(BurgerPlateComponent&& other) = delete;
	
		void OnBurgerPieceHit(CollisionComponent* otherColComp, GameObject* otherGOCol);
	};
	

}