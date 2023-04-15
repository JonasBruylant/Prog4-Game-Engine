#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"

#include <steam_api.h>
#include <string>

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

#include "InputManager.h"

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
	glm::vec2 position, glm::vec2 healthUIPos, glm::vec2 scoreUIPos, std::string imagePath, SDL_Color uiColor, bool isKeyBoard);
std::shared_ptr<dae::GameObject> AddStandardPlayerObject(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, unsigned int playerIndex,
		float x, float y, float healthX, float healthY, float scoreX, float scoreY, std::string imagePath, SDL_Color uiColor, bool isKeyBoard);



std::shared_ptr<dae::GameObject> AddPlayerObjectWithScoreAndHealth(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, unsigned int playerIndex,
	glm::vec2 position, glm::vec2 healthUIPos, glm::vec2 scoreUIPos, std::string imagePath,
	SDL_Scancode keyboardKillButton, SDL_Scancode keyBoardGainScorebutton,
	dae::Controller::ControllerButton controllerKillButton, dae::Controller::ControllerButton controllerGainScoreButton, SDL_Color uiColor, bool isKeyBoard);

std::shared_ptr<dae::GameObject> AddPlayerObjectWithScoreAndHealth(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, unsigned int playerIndex,
		float x, float y, float healthX, float healthY, float scoreX, float scoreY, std::string imagePath,
		SDL_Scancode keyboardKillButton, SDL_Scancode keyBoardGainScorebutton,
		dae::Controller::ControllerButton controllerKillButton, dae::Controller::ControllerButton controllerGainScoreButton, SDL_Color uiColor, bool isKeyBoard);





void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto& resourceManager = dae::ResourceManager::GetInstance();
	auto font = resourceManager.LoadFont("Lingua.otf", 36);
	auto texture = resourceManager.LoadTexture("background.tga");
	auto& inputManager = dae::InputManager::GetInstance();


	auto go = std::make_shared<dae::GameObject>();
	go->Initialize();
	go->AddComponent<dae::ImageObjectComponent>()->SetTexture(texture);
	go->AddComponent<dae::ImageRenderComponent>();

	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(0, 0, 0);
	scene.Add(go);


	go = std::make_shared<dae::GameObject>();
	go = AddTextGameObject(scene, resourceManager, 0.f, 0.f, "0", "Lingua.otf", 36);
	go->AddComponent<dae::FPSCountComponent>();
	scene.Add(go);



	go = AddPlayerObjectWithScoreAndHealth(scene, resourceManager, inputManager, 0,{ 200, 200 }, { 10, 200 }, { 10, 230 }, "Galaga_Bee.png",
		SDL_SCANCODE_X, SDL_SCANCODE_Z, dae::Controller::ControllerButton::ButtonA, dae::Controller::ControllerButton::ButtonB,{255,255,0}, true);
	go = AddPlayerObjectWithScoreAndHealth(scene, resourceManager, inputManager, 1,{ 250, 200 }, { 10, 280 }, { 10, 310 }, "Galaga_Butterfly.png", 
		SDL_SCANCODE_Q, SDL_SCANCODE_E, dae::Controller::ControllerButton::ButtonX, dae::Controller::ControllerButton::ButtonY,{0,255,255}, false);
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
	auto go = std::make_shared<dae::GameObject>();
	go->Initialize();

	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(x, y, 0);

	scene.Add(go);

	return go;
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
	auto font = resourceManager.LoadFont(fontPath, fontSize);

	auto go = std::make_shared<dae::GameObject>();
	go->Initialize();
	go->AddComponent<dae::TextObjectComponent>()->SetFont(font);
	go->GetComponent<dae::TextObjectComponent>()->SetText(text);
	go->GetComponent<dae::TextObjectComponent>()->SetColor(color.r, color.g, color.b);
	go->AddComponent<dae::TextRenderComponent>();

	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(x, y, 0);

	scene.Add(go);

	return go;
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
	auto go = std::make_shared<dae::GameObject>();
	auto texture = resourceManager.LoadTexture(imagePath);
	go->Initialize();
	go->AddComponent<dae::ImageObjectComponent>()->SetTexture(texture);
	go->AddComponent<dae::ImageRenderComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(x, y, 0);

	scene.Add(go);

	
	return go;

}


//SDL_Scancode
std::shared_ptr<dae::GameObject> AddStandardPlayerObject(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager,unsigned int playerIndex,
	glm::vec2 position, glm::vec2 healthUIPos, glm::vec2 scoreUIPos ,std::string imagePath,  SDL_Color uiColor, bool isKeyBoard)
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

	inputManager.AddControllerCommandToMap<dae::DieCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::ButtonB, dae::ButtonState::Down }
	, std::make_unique<dae::DieCommand>(playerHealth.get()));

	inputManager.AddKeyboardCommandToMap<dae::DieCommand>(dae::KeyBoardInput{ SDL_SCANCODE_Z, dae::ButtonState::Down }, std::make_unique<dae::DieCommand>(playerHealth.get()));

	//Lives remaining
	go = AddTextGameObject(scene, resourceManager, healthUIPos.x, healthUIPos.y, "This is a test", "Lingua.otf", 24, uiColor); //10, 200
	auto livesRemanainingObserver = go->AddComponent<dae::LivesRemainingUIComponent>();

	playerHealth->GetPlayerDiedEventSubject()->AddObserver(livesRemanainingObserver.get());

	//Score
	go = AddTextGameObject(scene, resourceManager, scoreUIPos.x, scoreUIPos.y, "Score: ", "Lingua.otf", 24, uiColor); // 10, 230
	auto scoreObserver = go->AddComponent<dae::ScoreUIComponent>();

	playerScore->GetScoreGainedSubject()->AddObserver(scoreObserver.get());

	return toReturnObject;
}

std::shared_ptr<dae::GameObject> AddStandardPlayerObject(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager,unsigned int playerIndex,
	float x, float y, float healthX, float healthY, float scoreX, float scoreY, std::string imagePath, SDL_Color uiColor, bool isKeyBoard)
{
	auto go = std::make_shared<dae::GameObject>();
	auto texture = resourceManager.LoadTexture(imagePath);
	go->Initialize();
	go->AddComponent<dae::ImageObjectComponent>()->SetTexture(texture);
	go->AddComponent<dae::ImageRenderComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(x, y, 0);
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

	inputManager.AddControllerCommandToMap<dae::DieCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::ButtonB, dae::ButtonState::Down }
	, std::make_unique<dae::DieCommand>(playerHealth.get()));

	inputManager.AddKeyboardCommandToMap<dae::DieCommand>(dae::KeyBoardInput{ SDL_SCANCODE_Z, dae::ButtonState::Down }, std::make_unique<dae::DieCommand>(playerHealth.get()));

//Lives remaining
	go = AddTextGameObject(scene, resourceManager, healthX, healthY, "This is a test", "Lingua.otf", 24, uiColor);
	auto livesRemanainingObserver = go->AddComponent<dae::LivesRemainingUIComponent>();

	playerHealth->GetPlayerDiedEventSubject()->AddObserver(livesRemanainingObserver.get());

//Score
	go = AddTextGameObject(scene, resourceManager, scoreX, scoreY, "Score: ", "Lingua.otf", 24, uiColor);
	auto scoreObserver = go->AddComponent<dae::ScoreUIComponent>();

	playerScore->GetScoreGainedSubject()->AddObserver(scoreObserver.get());

	return toReturnObject;
}



std::shared_ptr<dae::GameObject> AddPlayerObjectWithScoreAndHealth(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager,unsigned int playerIndex,
	glm::vec2 position, glm::vec2 healthUIPos, glm::vec2 scoreUIPos, std::string imagePath, 
	SDL_Scancode keyboardKillButton, SDL_Scancode keyBoardGainScorebutton, 
	dae::Controller::ControllerButton controllerKillButton, dae::Controller::ControllerButton controllerGainScoreButton, SDL_Color uiColor, bool isKeyBoard)
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

	return toReturnObject;
}

std::shared_ptr<dae::GameObject> AddPlayerObjectWithScoreAndHealth(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager,unsigned int playerIndex,
	float x, float y, float healthX, float healthY, float scoreX, float scoreY, std::string imagePath,
	SDL_Scancode keyboardKillButton, SDL_Scancode keyBoardGainScorebutton, 
	dae::Controller::ControllerButton controllerKillButton, dae::Controller::ControllerButton controllerGainScoreButton, SDL_Color uiColor, bool isKeyBoard)
{
	auto go = std::make_shared<dae::GameObject>();
	auto texture = resourceManager.LoadTexture(imagePath);
	go->Initialize();
	go->AddComponent<dae::ImageObjectComponent>()->SetTexture(texture);
	go->AddComponent<dae::ImageRenderComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(x, y, 0);
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
	go = AddTextGameObject(scene, resourceManager, healthX, healthY, "This is a test", "Lingua.otf", 24, uiColor);
	auto livesRemanainingObserver = go->AddComponent<dae::LivesRemainingUIComponent>();

	playerHealth->GetPlayerDiedEventSubject()->AddObserver(livesRemanainingObserver.get());

	//Score
	go = AddTextGameObject(scene, resourceManager, scoreX, scoreY, "Score: ", "Lingua.otf", 24, uiColor);
	auto scoreObserver = go->AddComponent<dae::ScoreUIComponent>();

	playerScore->GetScoreGainedSubject()->AddObserver(scoreObserver.get());

	return toReturnObject;
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
