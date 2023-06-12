#include "MuteSoundCommand.h"
#include "SoundManager.h"
#include "SoundSystem.h"

void dae::MuteSoundCommand::Execute()
{
	SoundManager::GetInstance().GetSoundSystem()->Mute();
}
