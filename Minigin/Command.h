#pragma once
#include <memory>

namespace dae {

class GameObject;

class Command
{
public:
	enum class BoundButtonState
	{
		Up,
		Down,
		Pressed
	};

	virtual ~Command() = default;
	Command(const Command& other) = delete;
	Command(Command&& other) = delete;
	Command& operator=(const Command& other) = delete;
	Command& operator=(Command&& other) = delete;

	virtual void Execute(float) = 0;

	BoundButtonState GetBoundButtonState() { return m_selectedButtonState; }
	void SetBoundButtonState(BoundButtonState  buttonState) { m_selectedButtonState = buttonState; }


protected:
	explicit Command() :m_selectedButtonState{BoundButtonState::Pressed} {	}
private:
	BoundButtonState m_selectedButtonState;
};
}

