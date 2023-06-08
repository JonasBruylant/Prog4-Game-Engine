#pragma once
#include "SoundSystem.h"
#include <string>
#include <memory>
#include <queue>
#include <mutex>

namespace dae 
{

class SDLSoundSystem final : public SoundSystem
{

	class SDLSoundSystemImpl;
	std::unique_ptr<SDLSoundSystemImpl> m_pSoundImpl = nullptr;
public:

	SDLSoundSystem();
	~SDLSoundSystem();

	SDLSoundSystem(const SDLSoundSystem& other) = delete;
	SDLSoundSystem(SDLSoundSystem&& other) = delete;
	SDLSoundSystem& operator=(const SDLSoundSystem& other) = delete;
	SDLSoundSystem& operator=(SDLSoundSystem&& other) = delete;


	void Init(const std::string& dataPath) override;
	void Quit() override;

	void LoadSound(SoundData data) override;
	void PlaySound(SoundData data) override;
	bool CheckIsSoundLoaded(unsigned short id) override;

	void NotifyQueue(SoundData data);
	void RunThread();

private:

	std::string m_dataPath;

	std::queue<SoundData> m_soundEventQueue;
	std::mutex m_mutex;
	std::condition_variable m_queueCondition;
	std::atomic<bool> m_isThreadRunning;

	std::jthread m_soundThread;
};
}

