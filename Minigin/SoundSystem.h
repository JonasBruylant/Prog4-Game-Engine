#pragma once
#include <string>

namespace dae 
{
	struct SoundData
	{
		enum class SoundType
		{
			SoundTrack,
			SoundEffect
		};

		unsigned short id{};
		float volume = 1.f;
		std::string filePath = "";
		SoundType soundType = SoundType::SoundEffect;
		bool loadFile = false;
	};

class SoundSystem
{
public:

	SoundSystem() = default;
	virtual ~SoundSystem() = default;

	SoundSystem(const SoundSystem& other) = delete;
	SoundSystem(SoundSystem&& other) = delete;
	SoundSystem& operator=(const SoundSystem& other) = delete;
	SoundSystem& operator=(SoundSystem&& other) = delete;

	virtual void Init(const std::string& dataPath) = 0;
	virtual void Quit() = 0;
	
	virtual void LoadSound(SoundData soundData) = 0;
	virtual void PlaySound(SoundData soundData) = 0;
	virtual bool CheckIsSoundLoaded(unsigned short id) = 0;

	virtual void NotifyQueue(SoundData soundData) = 0;
};


}