#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif
#include "Minigin.h"
#include <string>

//SingleTons
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "InputManager.h"

//Steam
#include <steam_api.h>
#include "Achievements.h"

//Components
#include "TextObjectComponent.h"
#include "Transform.h"
#include "TextRenderComponent.h"
#include "FPSCountComponent.h"
#include "ImageObjectComponent.h"
#include "ImageRenderComponent.h"
#include "RotatorComponent.h"
#include "TrashTheCacheComponent.h"
#include "HealthComponent.h"
#include "LivesRemainingUIComponent.h"
#include "ScoreComponent.h"
#include "ScoreUIComponent.h"
#include "TutorialComponent.h"

//Commands
#include "Controller.h"
#include "Command.h"
#include "MoveCommand.h"
#include "DieCommand.h"
#include "GainScoreCommand.h"

std::shared_ptr<dae::GameObject> AddEmptyGameObject(dae::Scene& scene, glm::vec2 position);

std::shared_ptr<dae::GameObject> AddTextGameObject(dae::Scene& scene, dae::ResourceManager& resourceManager, glm::vec2 position, std::string text, std::string fontPath, int fontSize, SDL_Color color = { 255,255,255 });
std::shared_ptr<dae::GameObject> AddTextGameObject(dae::Scene& scene, dae::ResourceManager& resourceManager, float x, float y, std::string text, std::string fontPath, int fontSize, SDL_Color color = { 255,255,255 });

std::shared_ptr<dae::GameObject> AddImageGameObject(dae::Scene& scene, dae::ResourceManager& resourceManager, glm::vec2 position, std::string imagePath);
std::shared_ptr<dae::GameObject> AddImageGameObject(dae::Scene& scene, dae::ResourceManager& resourceManager, float x, float y, std::string imagePath);

std::shared_ptr<dae::GameObject> AddStandardPlayerObject(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, unsigned int playerIndex,
	glm::vec2 position, std::string imagePath, bool isKeyBoard);
std::shared_ptr<dae::GameObject> AddStandardPlayerObject(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, unsigned int playerIndex,
		float x, float y, std::string imagePath, bool isKeyBoard);



std::shared_ptr<dae::GameObject> AddPlayerObjectWithScoreAndHealth(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, unsigned int playerIndex,
	glm::vec2 position, glm::vec2 healthUIPos, glm::vec2 scoreUIPos, std::string imagePath,
	SDL_Scancode keyboardKillButton, SDL_Scancode keyBoardGainScorebutton,
	dae::Controller::ControllerButton controllerKillButton, dae::Controller::ControllerButton controllerGainScoreButton,
	std::shared_ptr<dae::SteamAchievements> achievements,
	SDL_Color uiColor, bool isKeyBoard);

std::shared_ptr<dae::GameObject> AddPlayerObjectWithScoreAndHealth(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, unsigned int playerIndex,
		float x, float y, float healthX, float healthY, float scoreX, float scoreY, std::string imagePath,
		SDL_Scancode keyboardKillButton, SDL_Scancode keyBoardGainScorebutton,
		dae::Controller::ControllerButton controllerKillButton, dae::Controller::ControllerButton controllerGainScoreButton, 
		std::shared_ptr<dae::SteamAchievements> achievements,
		SDL_Color uiColor, bool isKeyBoard);





void load()
{
	using namespace dae;
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	auto& resourceManager = ResourceManager::GetInstance();
	auto font = resourceManager.LoadFont("Lingua.otf", 36);
	auto texture = resourceManager.LoadTexture("background.tga");
	auto& inputManager = InputManager::GetInstance();


	auto go = std::make_shared<GameObject>();
	go->Initialize();
	go->AddComponent<ImageObjectComponent>()->SetTexture(texture);
	go->AddComponent<ImageRenderComponent>();

	go->GetComponent<TransformComponent>()->SetLocalPosition(0, 0, 0);
	scene.Add(go);


	go = std::make_shared<GameObject>();
	go = AddTextGameObject(scene, resourceManager, 0.f, 0.f, "0", "Lingua.otf", 36);
	go->AddComponent<FPSCountComponent>();
	scene.Add(go);

	std::vector<Achievement_t> achievementsList{ {EAchievements::ACH_WIN_ONE_GAME, "ACH_WIN_ONE_GAME"}, {EAchievements::ACH_WIN_100_GAMES, "ACH_WIN_100_GAMES"} };
	auto achievement = std::make_shared<SteamAchievements>(achievementsList);

	go = AddPlayerObjectWithScoreAndHealth(scene, resourceManager, inputManager, 0,{ 200, 200 }, { 10, 200 }, { 10, 230 }, "Galaga_Bee.png",
		SDL_SCANCODE_X, SDL_SCANCODE_Z,Controller::ControllerButton::ButtonA, Controller::ControllerButton::ButtonB, achievement, { 255,255,0 }, true);
	go = AddPlayerObjectWithScoreAndHealth(scene, resourceManager, inputManager, 0, { 250, 200 }, { 10, 280 }, { 10, 310 }, "Galaga_Butterfly.png",
		SDL_SCANCODE_Q, SDL_SCANCODE_E, Controller::ControllerButton::ButtonX, Controller::ControllerButton::ButtonY, achievement,{0,255,255}, false);


	go = AddEmptyGameObject(scene, { 20,20 });
	go->AddComponent<TutorialComponent>();
}

std::shared_ptr<dae::GameObject> AddEmptyGameObject(dae::Scene& scene, glm::vec2 position)
{
	auto go = std::make_shared<dae::GameObject>();
	go->Initialize();

	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x, position.y, 0);

	scene.Add(go);


	return go;
}
std::shared_ptr<dae::GameObject> AddEmptyGameObject(dae::Scene& scene, float x, float y)
{
	return AddEmptyGameObject(scene, { x,y });
}

std::shared_ptr<dae::GameObject> AddTextGameObject(dae::Scene& scene, dae::ResourceManager& resourceManager, glm::vec2 position, std::string text ,std::string fontPath, int fontSize, SDL_Color color)
{
	auto font = resourceManager.LoadFont(fontPath, fontSize);
	auto go = std::make_shared<dae::GameObject>();
	go->Initialize();
	
	go->AddComponent<dae::TextObjectComponent>()->SetFont(font);
	go->GetComponent<dae::TextObjectComponent>()->SetText(text);
	go->GetComponent<dae::TextObjectComponent>()->SetColor(color.r, color.g, color.b);
	go->AddComponent<dae::TextRenderComponent>();

	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x, position.y, 0);

	scene.Add(go);

	return go;
}
std::shared_ptr<dae::GameObject> AddTextGameObject(dae::Scene& scene, dae::ResourceManager& resourceManager, float x, float y, std::string text, std::string fontPath, int fontSize, SDL_Color color)
{
	return AddTextGameObject(scene, resourceManager, { x,y }, text, fontPath, fontSize, color);
}

std::shared_ptr<dae::GameObject> AddImageGameObject(dae::Scene& scene, dae::ResourceManager& resourceManager, glm::vec2 position, std::string imagePath)
{
	auto go = std::make_shared<dae::GameObject>();
	auto texture = resourceManager.LoadTexture(imagePath);
	go->Initialize();
	go->AddComponent<dae::ImageObjectComponent>()->SetTexture(texture);
	go->AddComponent<dae::ImageRenderComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x, position.y, 0);
	
	scene.Add(go);

	return go;

}
std::shared_ptr<dae::GameObject> AddImageGameObject(dae::Scene& scene, dae::ResourceManager& resourceManager, float x, float y, std::string imagePath)
{
	return AddImageGameObject(scene, resourceManager, { x,y }, imagePath);
}


//SDL_Scancode
std::shared_ptr<dae::GameObject> AddStandardPlayerObject(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager,unsigned int playerIndex,
	glm::vec2 position, std::string imagePath, bool isKeyBoard)
{
	auto go = std::make_shared<dae::GameObject>();
	auto texture = resourceManager.LoadTexture(imagePath);
	go->Initialize();
	go->AddComponent<dae::ImageObjectComponent>()->SetTexture(texture);
	go->AddComponent<dae::ImageRenderComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x, position.y, 0);
	go->AddComponent<dae::HealthComponent>();

	int movementSpeed = 100;
	if (!isKeyBoard)
	{
		auto pMoveCommand = inputManager.AddControllerCommandToMap<dae::MoveCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadUp, dae::ButtonState::Pressed }
		, std::make_unique<dae::MoveCommand>(go));

		pMoveCommand->SetDirection({ 0,-1 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::MoveCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadDown, dae::ButtonState::Pressed }
		, std::make_unique<dae::MoveCommand>(go));

		pMoveCommand->SetDirection({ 0,1 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::MoveCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadRight, dae::ButtonState::Pressed }
		, std::make_unique<dae::MoveCommand>(go));

		pMoveCommand->SetDirection({ 1,0 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::MoveCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadLeft, dae::ButtonState::Pressed }
		, std::make_unique<dae::MoveCommand>(go));

		pMoveCommand->SetDirection({ -1,0 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

	}
	else
	{
		auto pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::MoveCommand>(dae::KeyBoardInput{ SDL_SCANCODE_S, dae::ButtonState::Pressed }, std::make_unique<dae::MoveCommand>(go));
		pMoveCommand->SetDirection({ 0,1 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::MoveCommand>(dae::KeyBoardInput{ SDL_SCANCODE_W, dae::ButtonState::Pressed }, std::make_unique<dae::MoveCommand>(go));
		pMoveCommand->SetDirection({ 0,-1 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::MoveCommand>(dae::KeyBoardInput{ SDL_SCANCODE_D, dae::ButtonState::Pressed }, std::make_unique<dae::MoveCommand>(go));
		pMoveCommand->SetDirection({ 1,0 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::MoveCommand>(dae::KeyBoardInput{ SDL_SCANCODE_A, dae::ButtonState::Pressed }, std::make_unique<dae::MoveCommand>(go));
		pMoveCommand->SetDirection({ -1,0 });
		pMoveCommand->SetMovementSpeed(movementSpeed);
	}

	scene.Add(go);

	return go;
}

std::shared_ptr<dae::GameObject> AddStandardPlayerObject(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager,unsigned int playerIndex,
	float x, float y, std::string imagePath, bool isKeyBoard)
{
	return AddStandardPlayerObject(scene, resourceManager, inputManager, playerIndex, { x,y }, imagePath, isKeyBoard);
}



std::shared_ptr<dae::GameObject> AddPlayerObjectWithScoreAndHealth(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager,unsigned int playerIndex,
	glm::vec2 position, glm::vec2 healthUIPos, glm::vec2 scoreUIPos, std::string imagePath, 
	SDL_Scancode keyboardKillButton, SDL_Scancode keyBoardGainScorebutton, 
	dae::Controller::ControllerButton controllerKillButton, dae::Controller::ControllerButton controllerGainScoreButton,
	std::shared_ptr<dae::SteamAchievements> achievements,
	SDL_Color uiColor, bool isKeyBoard)
{
	auto go = std::make_shared<dae::GameObject>();
	auto texture = resourceManager.LoadTexture(imagePath);
	go->Initialize();
	go->AddComponent<dae::ImageObjectComponent>()->SetTexture(texture);
	go->AddComponent<dae::ImageRenderComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x, position.y, 0);
	auto playerHealth = go->AddComponent<dae::HealthComponent>();
	auto playerScore = go->AddComponent<dae::ScoreComponent>();

	int movementSpeed = 100;
	if (!isKeyBoard)
	{
		auto pMoveCommand = inputManager.AddControllerCommandToMap<dae::MoveCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadUp, dae::ButtonState::Pressed }
		, std::make_unique<dae::MoveCommand>(go));

		pMoveCommand->SetDirection({ 0,-1 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::MoveCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadDown, dae::ButtonState::Pressed }
		, std::make_unique<dae::MoveCommand>(go));

		pMoveCommand->SetDirection({ 0,1 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::MoveCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadRight, dae::ButtonState::Pressed }
		, std::make_unique<dae::MoveCommand>(go));

		pMoveCommand->SetDirection({ 1,0 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::MoveCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadLeft, dae::ButtonState::Pressed }
		, std::make_unique<dae::MoveCommand>(go));

		pMoveCommand->SetDirection({ -1,0 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

	}
	else
	{
		auto pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::MoveCommand>(dae::KeyBoardInput{ SDL_SCANCODE_S, dae::ButtonState::Pressed }, std::make_unique<dae::MoveCommand>(go));
		pMoveCommand->SetDirection({ 0,1 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::MoveCommand>(dae::KeyBoardInput{ SDL_SCANCODE_W, dae::ButtonState::Pressed }, std::make_unique<dae::MoveCommand>(go));
		pMoveCommand->SetDirection({ 0,-1 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::MoveCommand>(dae::KeyBoardInput{ SDL_SCANCODE_D, dae::ButtonState::Pressed }, std::make_unique<dae::MoveCommand>(go));
		pMoveCommand->SetDirection({ 1,0 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::MoveCommand>(dae::KeyBoardInput{ SDL_SCANCODE_A, dae::ButtonState::Pressed }, std::make_unique<dae::MoveCommand>(go));
		pMoveCommand->SetDirection({ -1,0 });
		pMoveCommand->SetMovementSpeed(movementSpeed);
	}

	auto toReturnObject = go;
	scene.Add(go);

	//Die command
	inputManager.AddControllerCommandToMap<dae::DieCommand>(dae::ControllerInput{ playerIndex, controllerKillButton, dae::ButtonState::Down }, std::make_unique<dae::DieCommand>(playerHealth.get()));

	inputManager.AddKeyboardCommandToMap<dae::DieCommand>(dae::KeyBoardInput{ keyboardKillButton, dae::ButtonState::Down }, std::make_unique<dae::DieCommand>(playerHealth.get()));


	//Gain score command
	inputManager.AddControllerCommandToMap<dae::GainScoreCommand>(dae::ControllerInput{ playerIndex, controllerGainScoreButton, dae::ButtonState::Down }, std::make_unique<dae::GainScoreCommand>(playerScore.get()));

	inputManager.AddKeyboardCommandToMap<dae::GainScoreCommand>(dae::KeyBoardInput{ keyBoardGainScorebutton, dae::ButtonState::Down }, std::make_unique<dae::GainScoreCommand>(playerScore.get()));

	//Lives remaining
	go = AddTextGameObject(scene, resourceManager, healthUIPos.x, healthUIPos.y, "This is a test", "Lingua.otf", 24, uiColor); //10, 200
	auto livesRemanainingObserver = go->AddComponent<dae::LivesRemainingUIComponent>();

	playerHealth->GetPlayerDiedEventSubject()->AddObserver(livesRemanainingObserver.get());

	//Score
	go = AddTextGameObject(scene, resourceManager, scoreUIPos.x, scoreUIPos.y, "Score: ", "Lingua.otf", 24, uiColor); // 10, 230
	auto scoreObserver = go->AddComponent<dae::ScoreUIComponent>();

	playerScore->GetScoreGainedSubject()->AddObserver(scoreObserver.get());


	//Achievements
	scoreObserver->SetAchievement(achievements);
	return toReturnObject;
}

std::shared_ptr<dae::GameObject> AddPlayerObjectWithScoreAndHealth(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager,unsigned int playerIndex,
	float x, float y, float healthX, float healthY, float scoreX, float scoreY, std::string imagePath,
	SDL_Scancode keyboardKillButton, SDL_Scancode keyBoardGainScorebutton, 
	dae::Controller::ControllerButton controllerKillButton, dae::Controller::ControllerButton controllerGainScoreButton, 
	std::shared_ptr<dae::SteamAchievements> achievements,
	SDL_Color uiColor, bool isKeyBoard)
{
	return AddPlayerObjectWithScoreAndHealth(scene, resourceManager, inputManager, playerIndex, { x,y }, { healthX, healthY }, {scoreX, scoreY}, imagePath,
		keyboardKillButton, keyBoardGainScorebutton,
		controllerKillButton, controllerGainScoreButton, achievements,
		uiColor, isKeyBoard);
}

int main(int, char* []) {

	if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
		return 1;
	}
	else
		std::cout << "Successfully initialized steam." << std::endl;


	dae::Minigin engine("../Data/");
	engine.Run(load);

	SteamAPI_Shutdown();
	return 0;
}
