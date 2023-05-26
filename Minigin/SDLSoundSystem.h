#pragma once
#include "SoundSystem.h"
#include <memory>
#include <string>

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


	void Init() override;
	void Quit() override;

	void LoadSound(unsigned short id, const std::string& filePath) override;
	void PlaySound(unsigned short id, SoundType soundType, float volume) override;
	bool CheckIsSoundLoaded(unsigned short id) override;
};
}

