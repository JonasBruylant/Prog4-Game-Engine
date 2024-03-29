#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Minigin.h"

#include "InputManager.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "ResourceManager.h"
#include "../BurgerTime/JsonFileReader.h"
#include "../BurgerTime/HighscoreManager.h"

#include "Renderer.h"
#include "Timer.h"
#include <chrono>
#include "SDLSoundSystem.h"

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string &dataPath, float windowWidth, float windowHeight)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,									
		static_cast<int>(windowWidth),											
		static_cast<int>(windowHeight),											
		SDL_WINDOW_OPENGL										
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);
	JsonFileReader::GetInstance().Init(dataPath);
	HighscoreManager::GetInstance().Init("Highscores.json");
	SoundManager::GetInstance().SetSoundSystem(std::make_unique<SDLSoundSystem>());
	SoundManager::GetInstance().Initialize(dataPath);
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SoundManager::GetInstance().Quit();
	SceneManager::GetInstance().Quit();
	HighscoreManager::GetInstance().Quit();

	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();


	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& timer = Timer::GetInstance();

	bool doContinue = true;
	auto lastTime = std::chrono::high_resolution_clock::now();

	while (doContinue)
	{
		const auto currentTime = std::chrono::high_resolution_clock::now();
		const float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();

		timer.SetDeltaTime(deltaTime);
		doContinue = input.ProcessInput();
		sceneManager.Update();
		renderer.Render();

		lastTime = currentTime;

		const auto sleepTime = currentTime + std::chrono::milliseconds(timer.GetFrameTimeMS()) - std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(sleepTime);
	}
}
