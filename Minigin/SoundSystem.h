#pragma once
#include <memory>
#include <string>


namespace dae {

class SoundSystem
{
	class SoundSystemImpl;
	std::unique_ptr<SoundSystemImpl> m_pSoundImpl = nullptr;
public:
	enum class SoundType
	{
		SoundTrack,
		SoundEffect,
	};

	SoundSystem();
	~SoundSystem();

	SoundSystem(const SoundSystem& other) = delete;
	SoundSystem(SoundSystem&& other) = delete;
	SoundSystem& operator=(const SoundSystem& other) = delete;
	SoundSystem& operator=(SoundSystem&& other) = delete;

	void Init();
	void Quit();

	void LoadSound(unsigned short id, const std::string& filePath);
	void PlaySound(unsigned short id, SoundType soundType, float volume);
	bool CheckIsSoundLoaded(unsigned short id);
};
}

