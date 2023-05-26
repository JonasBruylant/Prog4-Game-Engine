#pragma once
#include "SoundSystem.h"

namespace dae {

class NullSoundSystem final : public SoundSystem
{

public:
	void Init() override{}
	void Quit() override{}
	
	void LoadSound(unsigned short, const std::string&) override {}
	void PlaySound(unsigned short, SoundType , float ) override{}
	bool CheckIsSoundLoaded(unsigned short) override { return false; }
};


}