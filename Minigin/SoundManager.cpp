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
	m_isThreadRunning = false;
	m_pSoundSystem->Quit();
}

void dae::SoundManager::Initialize(const std::string& dataPath)
{
	m_dataPath = dataPath;

	m_pSoundSystem->Init();

	m_isThreadRunning = true;
	m_soundThread = std::jthread(&SoundManager::RunThread, this);
}

void dae::SoundManager::Quit()
{
	m_pSoundSystem->Quit();
	m_isThreadRunning = false;

	m_queueCondition.notify_all();
}

void dae::SoundManager::NotifyQueue(SoundData data)				
{																
	std::lock_guard<std::mutex> guardLock(m_mutex);				
	data.filePath = m_dataPath + data.filePath;					
	m_soundEventQueue.push(data);								
																
	m_queueCondition.notify_all();								
}

void dae::SoundManager::SetSoundSystem(std::unique_ptr<SoundSystem> soundsystem)
{
	m_pSoundSystem->Quit();

	m_pSoundSystem = std::move(soundsystem);
}

dae::SoundSystem* dae::SoundManager::GetoundSystem()
{
	return m_pSoundSystem.get();
}
																
void dae::SoundManager::RunThread()								
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

		
		auto playableSound = m_soundEventQueue.front();
		m_soundEventQueue.pop();

		lock.unlock();

		if (playableSound.loadFile)
			LoadSound(playableSound);
		else
			PlaySound(playableSound);
	
	}
}

void dae::SoundManager::PlaySound(SoundData data)
{
	//If the wanted sound is not loaded in, load it in before trying to play.
	if (!m_pSoundSystem->CheckIsSoundLoaded(data.id))
	{
		m_pSoundSystem->LoadSound(data.id, data.filePath);
	}
	m_pSoundSystem->PlaySound(data.id, static_cast<SoundSystem::SoundType>(static_cast<int>(data.soundType)), data.volume);
}

void dae::SoundManager::LoadSound(SoundData data)
{
	//Prevent the another sound being loaded onto the same ID
	if (!m_pSoundSystem->CheckIsSoundLoaded(data.id))
	{
		m_pSoundSystem->LoadSound(data.id, data.filePath);
	}
}
