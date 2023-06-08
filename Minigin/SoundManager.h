#pragma once
#include "Singleton.h"
#include <string>
#include <memory>
#include <queue>
#include <mutex>

namespace dae 
{
	class SDLSoundSystem;
	class SoundSystem;

	class SoundManager final : public Singleton<SoundManager>
	{
	public:

		void Initialize(const std::string& dataPath);
		void Quit();

		void SetSoundSystem(std::unique_ptr<SoundSystem> soundsystem);
		SoundSystem* GetSoundSystem();
	private:
		friend class Singleton<SoundManager>;

		SoundManager();
		~SoundManager();

		SoundManager(const SoundManager& other) = delete;
		SoundManager(SoundManager&& other) = delete;
		SoundManager& operator=(const SoundManager& other) = delete;
		SoundManager& operator=(SoundManager&& other) = delete;


		std::unique_ptr<SoundSystem> m_pSoundSystem;
	};

}
