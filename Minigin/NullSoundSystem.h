#pragma once
#include "SoundSystem.h"

namespace dae {

class NullSoundSystem final : public SoundSystem
{

public:
	void Init(const std::string&) override{}
	void Quit() override{}
	
	void LoadSound(SoundData) override {}
	void PlaySound(SoundData) override{}
	bool CheckIsSoundLoaded(unsigned short) override { return false; }
	void NotifyQueue(SoundData) override {}
};


}