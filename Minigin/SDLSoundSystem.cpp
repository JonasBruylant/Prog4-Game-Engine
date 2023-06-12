#include "SDLSoundSystem.h"
#include "ResourceManager.h"

#include "SDL.h"
#include "SDL_mixer.h"
#include <unordered_map>
#include <stdexcept>

namespace dae
{
#pragma region SDLSoundSystem Impl
	class SDLSoundSystem::SDLSoundSystemImpl
	{
	
	public:

		void Init();
		void Quit();

		void PlaySound(unsigned short id, SoundData::SoundType soundType, float volume);
		void LoadSound(unsigned short id, const std::string& filePath);
		bool CheckIsSoundLoaded(unsigned short id);

		void Mute();
	private:
		bool m_isMuted{ false };
		std::unordered_map<unsigned short, Mix_Chunk*> m_LoadedSounds;
	};


	void SDLSoundSystem::SDLSoundSystemImpl::Init()
	{
		if (SDL_Init(SDL_INIT_AUDIO) < 0)
			throw std::runtime_error(std::string("Failed to initialize audio: ") + SDL_GetError());

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 4, 0) < 0)
			throw std::runtime_error("Failed to open audio: " + std::string(Mix_GetError()));

		//Soundtrack channel group 0 -> 0
		Mix_GroupChannels(0, 0, 0);
		//Soundeffect channel group 1 -> 3
		Mix_GroupChannels(1, 3, 1);
	}

	void SDLSoundSystem::SDLSoundSystemImpl::Quit()
	{
		Mix_CloseAudio();
	}

	void SDLSoundSystem::SDLSoundSystemImpl::Mute()
	{
		m_isMuted = !m_isMuted;

		if(m_isMuted)
			for (int i = 0; i < 4; ++i)
				Mix_Volume(i, 0);
		else
			for (int i = 0; i < 4; ++i)
				Mix_Volume(i, 128);
	}

	void SDLSoundSystem::SDLSoundSystemImpl::PlaySound(unsigned short id, SoundData::SoundType soundType, float volume)
	{
		if (!CheckIsSoundLoaded(id))
			throw std::runtime_error(std::string("Failed to find sound at given ID."));

		auto currentSound = m_LoadedSounds[id];
		Mix_VolumeChunk(currentSound, static_cast<int>(volume * MIX_MAX_VOLUME));


		int channel = 0;
		bool isRepeating = 1;
		if (soundType == SoundData::SoundType::SoundEffect)
		{
			//Check first available channel in the SFX group, if it's non-existent, overwrite the sound playing on channel 1
			channel = Mix_GroupAvailable(1);
			isRepeating = 0;

			if (channel == -1)
			{
				channel = 1;
			}
		}

		//Use SoundType to determine the channel it's supposed to use.
		Mix_PlayChannel(channel, currentSound, isRepeating);


	}

	void SDLSoundSystem::SDLSoundSystemImpl::LoadSound(unsigned short id, const std::string& filePath)
	{
		if (!CheckIsSoundLoaded(id))
		{
			Mix_Chunk* sound = Mix_LoadWAV(filePath.c_str());
			if (sound)
			{
				m_LoadedSounds[id] = sound;
				return;
			}

			throw std::runtime_error(std::string("Failed to load sound with given ID."));
		}
	}

	bool SDLSoundSystem::SDLSoundSystemImpl::CheckIsSoundLoaded(unsigned short id)
	{
		//Checks if the iterator is at the end, if it is, it's not loaded.
		return m_LoadedSounds.find(id) != m_LoadedSounds.end();
	}
#pragma endregion


#pragma region SoundSystem
	SDLSoundSystem::SDLSoundSystem()
	{
		m_dataPath = ResourceManager::GetInstance().GetResourceDataPath();
	}

	SDLSoundSystem::~SDLSoundSystem()
	{
	}
	//Worked on/Collaborated with Aaron Frans
	void SDLSoundSystem::Init(const std::string& dataPath)
	{
		m_dataPath = dataPath;
		m_pSoundImpl = std::make_unique<SDLSoundSystemImpl>();

		m_pSoundImpl->Init();

		m_isThreadRunning = true;
		m_soundThread = std::jthread(&SDLSoundSystem::RunThread, this);
	}
	void SDLSoundSystem::Quit()
	{
		m_pSoundImpl->Quit();
		m_isThreadRunning = false;

		m_queueCondition.notify_all();

	}
	void SDLSoundSystem::LoadSound(SoundData data)
	{
		m_pSoundImpl->LoadSound(data.id, data.filePath);
	}
	//Worked on/Collaborated with Aaron Frans
	void SDLSoundSystem::PlaySound(SoundData data)
	{
		if (!CheckIsSoundLoaded(data.id))
			m_pSoundImpl->LoadSound(data.id, data.filePath);

		m_pSoundImpl->PlaySound(data.id, data.soundType, data.volume);
	}
	//Worked on/Collaborated with Aaron Frans
	bool SDLSoundSystem::CheckIsSoundLoaded(unsigned short id)
	{
		return m_pSoundImpl->CheckIsSoundLoaded(id);
	}
	void SDLSoundSystem::Mute()
	{
		m_pSoundImpl->Mute();
	}
	//Worked on/Collaborated with Aaron Frans
	void SDLSoundSystem::NotifyQueue(SoundData data)
	{
		std::lock_guard<std::mutex> guardLock(m_mutex);
		data.filePath = m_dataPath + data.filePath;
		m_soundEventQueue.push(data);

		m_queueCondition.notify_all();
	}
	//Worked on/Collaborated with Aaron Frans
	void dae::SDLSoundSystem::RunThread()
	{
		while (m_isThreadRunning)
		{
			std::unique_lock<std::mutex> lock(m_mutex);

			//Only do something when queue is not empty.
			m_queueCondition.wait(lock, [&]()
			{
				if (!m_isThreadRunning)
					return true;

				return !m_soundEventQueue.empty();
			});

			if(m_soundEventQueue.empty())
				return ;

			SoundData playableSound = m_soundEventQueue.front();
			m_soundEventQueue.pop();

			lock.unlock();

			if (playableSound.loadFile)
				LoadSound(playableSound);
			else
				PlaySound(playableSound);

		}
	}
#pragma endregion

}