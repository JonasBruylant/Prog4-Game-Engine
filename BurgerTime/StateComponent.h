#pragma once
#include "Component.h"
#include "Structs.h"

namespace dae
{

	class StateComponent final : public Component
	{
	public:
		StateComponent(std::weak_ptr<GameObject> pOwner);

		~StateComponent() = default;
		StateComponent(const StateComponent& other) = delete;
		StateComponent(StateComponent&& other) = delete;
		StateComponent& operator=(const StateComponent& other) = delete;
		StateComponent& operator=(StateComponent&& other) = delete;

		void SetCurrentState(State state) { if(m_CurrentState != state) m_CurrentState = state; }
		void SetCurrentState(int state) { if (m_CurrentState != static_cast<State>(state)) m_CurrentState = static_cast<State>(state); }

		State GetCurrentState() { return m_CurrentState; }
		int GetCurrentStateInt() { return static_cast<int>(m_CurrentState); }
	private:
		State m_CurrentState{ State::Idle };
	};

}
