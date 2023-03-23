#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <XInput.h>
#pragma comment(lib, "xinput.lib")

#include "ControllerInput.h"
namespace dae 
{
	class ControllerInput::ControllerInputImpl
	{
		XINPUT_STATE m_previousState{};
		XINPUT_STATE m_currentState{};

		WORD m_buttonsPessedThisFrame;
		WORD m_buttonsReleasedThisFrame;

		int m_controllerIndex;

	public:
		ControllerInputImpl(unsigned int controllerIndex)
		{
			ZeroMemory(&m_previousState, sizeof(XINPUT_STATE));
			ZeroMemory(&m_currentState, sizeof(XINPUT_STATE));

			m_controllerIndex = controllerIndex;
		}

		void Update()
		{
			CopyMemory(&m_previousState, &m_currentState, sizeof(XINPUT_STATE));
			ZeroMemory(&m_currentState, sizeof(XINPUT_STATE));

			XInputGetState(m_controllerIndex, &m_currentState);

			auto buttonChanges = m_currentState.Gamepad.wButtons ^ m_previousState.Gamepad.wButtons;
			m_buttonsPessedThisFrame = buttonChanges & m_currentState.Gamepad.wButtons;
			m_buttonsReleasedThisFrame = buttonChanges & (~m_currentState.Gamepad.wButtons);
		}

		bool IsDownThisFrame(unsigned int button) const { return m_buttonsPessedThisFrame & button; }
		bool IsUpThisFrame(unsigned int button)const { return m_buttonsReleasedThisFrame & button; }
		bool IsPressed(unsigned int button) const { return m_currentState.Gamepad.wButtons & button; }

	};

	ControllerInput::ControllerInput(unsigned int controllerIndex)
	{
		//Change to something other than Raw Pointer
		pImpl = new ControllerInputImpl(controllerIndex);
	}
	ControllerInput::~ControllerInput()
	{
		//Change to something other than Raw Pointer
		delete pImpl;
	}

	void ControllerInput::Update()
	{
		pImpl->Update();
	}
	bool ControllerInput::IsDown(ControllerButton button) const
	{
		return pImpl->IsDownThisFrame(static_cast<unsigned int>(button));
	}
	bool ControllerInput::IsUp(ControllerButton button) const
	{
		return pImpl->IsUpThisFrame(static_cast<unsigned int>(button));
	}
	bool ControllerInput::IsPressed(ControllerButton button) const
	{
		return pImpl->IsPressed(static_cast<unsigned int>(button));
	}
}