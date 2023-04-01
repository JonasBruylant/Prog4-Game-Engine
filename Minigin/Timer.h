#pragma once
#include "Singleton.h"
namespace dae
{

	class Timer final : public Singleton<Timer>
	{
	public:
		Timer();
		~Timer();

		Timer(const Timer& other) = delete;
		Timer(Timer&& other) = delete;
		Timer& operator=(const Timer& other) = delete;
		Timer& operator=(Timer&& other) = delete;

		void Update();

		float GetDeltaTime() const;
		void SetDeltaTime(float deltaTime);
		int GetFrameTimeMS() const;
	private:

		float m_DeltaTime;

		const int m_DesiredFPS{ 144 };
		const int m_FrameTimeMS{ 1000 / m_DesiredFPS };
	};

}
