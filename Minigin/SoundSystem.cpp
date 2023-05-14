#include "SoundSystem.h"

#include "SDL.h"
#include "SDL_mixer.h"
#include <unordered_map>
#include <stdexcept>

namespace dae
{
#pragma region SoundSystem Impl
	class SoundSystem::SoundSystemImpl
	{
	
	public:

		void Init();
		void Quit();

		void PlaySound(unsigned short id, SoundType soundType, float volume);
		void LoadSound(unsigned short id, const std::string& filePath);
		bool CheckIsSoundLoaded(unsigned short id);
	private:
	
		std::unordered_map<unsigned short, Mix_Chunk*> m_LoadedSounds;
	};


	void SoundSystem::SoundSystemImpl::Init()
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

	void SoundSystem::SoundSystemImpl::Quit()
	{
		Mix_CloseAudio();
	}

	void SoundSystem::SoundSystemImpl::PlaySound(unsigned short id, SoundType soundType, float volume)
	{
		if (!CheckIsSoundLoaded(id))
			throw std::runtime_error(std::string("Failed to find sound at given ID."));

		auto currentSound = m_LoadedSounds[id];
		Mix_VolumeChunk(currentSound, static_cast<int>(volume * MIX_MAX_VOLUME));


		int channel = 0;
		bool isRepeating = 1;
		if (soundType == SoundType::SoundEffect)
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

	void SoundSystem::SoundSystemImpl::LoadSound(unsigned short id, const std::string& filePath)
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

	bool SoundSystem::SoundSystemImpl::CheckIsSoundLoaded(unsigned short id)
	{
		//Checks if the iterator is at the end, if it is, it's not loaded.
		return m_LoadedSounds.find(id) != m_LoadedSounds.end();
	}
#pragma endregion


#pragma region SoundSystem
	SoundSystem::SoundSystem()
	{
	}

	SoundSystem::~SoundSystem()
	{
	}

	void SoundSystem::Init()
	{
		m_pSoundImpl = std::make_unique<SoundSystemImpl>();

		m_pSoundImpl->Init();
	}
	void SoundSystem::Quit()
	{
		m_pSoundImpl->Quit();
	}
	void SoundSystem::LoadSound(unsigned short id, const std::string& filePath)
	{
		m_pSoundImpl->LoadSound(id, filePath);
	}
	void SoundSystem::PlaySound(unsigned short id, SoundType soundType, float volume)
	{
		m_pSoundImpl->PlaySound(id, soundType, volume);
	}
	bool SoundSystem::CheckIsSoundLoaded(unsigned short id)
	{
		return m_pSoundImpl->CheckIsSoundLoaded(id);
	}
#pragma endregion

}