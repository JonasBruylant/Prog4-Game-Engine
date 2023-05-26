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
	using soundId = unsigned short;
	
	//With this I can pass almost everything the sound needs and only access the value I want to access.
	struct SoundData
	{
		enum class SoundType
		{
			Soundtrack,
			SoundEffect
		};

		soundId id{};
		float volume = 1.f;
		std::string filePath =  "";
		SoundType soundType = SoundType::SoundEffect;
		bool loadFile = false;
	};

	class SoundManager final : public Singleton<SoundManager>
	{
	public:

		void Initialize(const std::string& dataPath);
		void Quit();

		//Pass the sound in this method to play the sound in a given ID
		//Call this when executing a command for attacking (Preferably on a component).
		void NotifyQueue(SoundData data);

		void SetSoundSystem(std::unique_ptr<SoundSystem> soundsystem);
		SoundSystem* GetoundSystem();
	private:
		friend class Singleton<SoundManager>;

		SoundManager();
		~SoundManager();

		SoundManager(const SoundManager& other) = delete;
		SoundManager(SoundManager&& other) = delete;
		SoundManager& operator=(const SoundManager& other) = delete;
		SoundManager& operator=(SoundManager&& other) = delete;



		std::string m_dataPath;

		std::unique_ptr<SoundSystem> m_pSoundSystem;
		std::queue<SoundData> m_soundEventQueue;
		std::mutex m_mutex;
		std::condition_variable m_queueCondition;
		std::atomic<bool> m_isThreadRunning;

		std::jthread m_soundThread;

		void PlaySound(SoundData data);
		void LoadSound(SoundData data);
		void RunThread();

	};

}
