#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <XInput.h>
#pragma comment(lib, "xinput.lib")

#include "Controller.h"
namespace dae 
{
	class Controller::ControllerImpl
	{
		XINPUT_STATE m_previousState{};
		XINPUT_STATE m_currentState{};

		WORD m_buttonsPessedThisFrame;
		WORD m_buttonsReleasedThisFrame;

		int m_controllerIndex;

	public:
		ControllerImpl(int controllerIndex)
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

	Controller::Controller(unsigned int controllerIndex)
	{
		//Change to something other than Raw Pointer
		pImpl = new ControllerImpl(controllerIndex);
	}
	Controller::~Controller()
	{
		//Change to something other than Raw Pointer
		delete pImpl;
	}

	void Controller::Update()
	{
		pImpl->Update();
	}
	bool Controller::IsDown(ControllerButton button) const
	{
		return pImpl->IsDownThisFrame(static_cast<unsigned int>(button));
	}
	bool Controller::IsUp(ControllerButton button) const
	{
		return pImpl->IsUpThisFrame(static_cast<unsigned int>(button));
	}
	bool Controller::IsPressed(ControllerButton button) const
	{
		return pImpl->IsPressed(static_cast<unsigned int>(button));
	}
}