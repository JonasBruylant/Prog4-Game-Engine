#pragma once
#include "GameObject.h"

namespace dae {

class Command
{
protected:
	GameObject* GetGameObject() const { return m_pActor; }
	explicit Command(GameObject* actor);

public:
	virtual ~Command();
	virtual void Execute() = 0;

private:
	GameObject* m_pActor;
};
}

