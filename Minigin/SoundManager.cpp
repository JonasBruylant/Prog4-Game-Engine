#include "SoundManager.h"
#include "SDLSoundSystem.h"
#include "SoundSystem.h"
#include "NullSoundSystem.h"



dae::SoundManager::SoundManager()
{
	m_pSoundSystem = std::make_unique<NullSoundSystem>();
}

dae::SoundManager::~SoundManager()
{
	m_pSoundSystem->Quit();
}

void dae::SoundManager::Initialize(const std::string& dataPath)
{
	//if (!m_pSoundSystem)
	//	m_pSoundSystem = std::make_unique<SoundSystem>();

	m_pSoundSystem->Init(dataPath);

}

void dae::SoundManager::Quit()
{
	m_pSoundSystem->Quit();
}

void dae::SoundManager::SetSoundSystem(std::unique_ptr<SoundSystem> soundsystem)
{
	m_pSoundSystem->Quit();

	m_pSoundSystem = std::move(soundsystem);
}

dae::SoundSystem* dae::SoundManager::GetSoundSystem()
{
	return m_pSoundSystem.get();
}