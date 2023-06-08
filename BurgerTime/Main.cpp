#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif
#include "Minigin.h"
#include <string>


//Sound system
#include "SoundSystem.h"

//Singletons
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "InputManager.h"
#include "SoundManager.h"

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
#include "CollisionComponent.h"
#include "StateComponent.h"
#include "AttackComponent.h"
#include "EnemyComponent.h"

//Commands
#include "Controller.h"
#include "Command.h"
#include "MoveCommand.h"
#include "DieCommand.h"
#include "GainScoreCommand.h"
#include "ChangeDirectionCommand.h"
#include "AttackCommand.h"

//Extras
#include "Texture2D.h"
#include "JsonLevelReader.h"
#include "Structs.h"

std::shared_ptr<dae::GameObject> AddEmptyGameObject(dae::Scene& scene, glm::vec2 position);

std::shared_ptr<dae::GameObject> AddTextGameObject(dae::Scene& scene, dae::ResourceManager& resourceManager, glm::vec2 position, std::string text, std::string fontPath, int fontSize, SDL_Color color = { 255,255,255 });

std::shared_ptr<dae::GameObject> AddImageGameObject(dae::Scene& scene, dae::ResourceManager& resourceManager, glm::vec2 position, std::string imagePath);

std::shared_ptr<dae::GameObject> AddStandardPlayerObject(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, unsigned int playerIndex,
	glm::vec2 position, std::string imagePath, bool isKeyBoard);

std::shared_ptr<dae::GameObject> AddPeterPepperPlayerObject(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, unsigned int playerIndex,
	glm::vec2 position, std::string imagePath, bool isKeyBoard);



std::shared_ptr<dae::GameObject> AddPlayerObjectWithScoreAndHealth(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, unsigned int playerIndex,
	glm::vec2 position, glm::vec2 healthUIPos, glm::vec2 scoreUIPos, std::string imagePath,
	SDL_Color uiColor, bool isKeyBoard);

void AddPeterPepper(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager);

void load()
{
	using namespace dae;
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	auto& resourceManager = ResourceManager::GetInstance();
	auto font = resourceManager.LoadFont("Lingua.otf", 36);
	auto texture = resourceManager.LoadTexture("background.tga");
	auto& inputManager = InputManager::GetInstance();
	auto& soundManager = SoundManager::GetInstance();
	auto& jsonLevelReader = JsonLevelReader::GetInstance();

	auto& lvl1Scene = jsonLevelReader.ReadAndLoadLevel("Level1.json");
	AddPeterPepper(lvl1Scene, resourceManager, inputManager);





	auto go = std::make_shared<GameObject>();
	go = AddTextGameObject(scene, resourceManager, { 0.f ,0.f }, "0", "Lingua.otf", 36);
	go->AddComponent<FPSCountComponent>();
	
	AddPeterPepper(scene, resourceManager, inputManager);
	
	//Enemy
	go = std::make_shared<GameObject>();
	go->Initialize();
	go->GetComponent<TransformComponent>()->SetLocalPosition({400, 200, 0});
	scene.Add(go);
	
	go->AddComponent<dae::ImageObjectComponent>();
	go->AddComponent<dae::ImageRenderComponent>();
	
	auto stateComp = go->AddComponent<StateComponent>();
	stateComp->SetCurrentState(State::Walking);
	auto goCollisionComponent = go->AddComponent<CollisionComponent>();
	goCollisionComponent->SetMeasurements(50.f,50.f);
	goCollisionComponent->SetTag("Enemy");
	goCollisionComponent->SetDebugColor({ 255,255,0 });
	
	auto enemyComponent = go->AddComponent<EnemyComponent>();
	enemyComponent->SetEnemyType(EnemyType::Egg);
	
	//Sound
	SoundData soundTrack{ 0,0.05f,"02_Game_Music.mp3", SoundData::SoundType::SoundTrack, false };
	soundManager.GetSoundSystem()->NotifyQueue(soundTrack);
	
	SoundData soundEffect{ 1, 0.1f,"Gain_score.mp3",  SoundData::SoundType::SoundEffect,true };
	soundManager.GetSoundSystem()->NotifyQueue(soundEffect);
}

void AddPeterPepper(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager)
{
	//Peter Pepper
	auto go = AddPeterPepperPlayerObject(scene, resourceManager, inputManager, 0, { 200, 200 }, "MrPepperDown.png", true);
	auto playerScore = go->AddComponent<dae::ScoreComponent>();
	auto playerHealth = go->AddComponent<dae::HealthComponent>();

	//Player Score UI
	go = AddTextGameObject(scene, resourceManager, { 10, 230 }, "Score: ", "Lingua.otf", 24, { 255,255,0 }); // 10, 230
	auto scoreObserver = go->AddComponent<dae::ScoreUIComponent>();

	playerScore->GetScoreGainedSubject()->AddObserver(scoreObserver.get());

	//Player Health UI
	go = AddTextGameObject(scene, resourceManager, { 10, 250 }, "Score: ", "Lingua.otf", 24, { 255,255,0 }); // 10, 230
	auto healthObserver = go->AddComponent<dae::LivesRemainingUIComponent>();

	playerHealth->GetPlayerDiedEventSubject()->AddObserver(healthObserver.get());
}

std::shared_ptr<dae::GameObject> AddEmptyGameObject(dae::Scene& scene, glm::vec2 position)
{
	auto go = std::make_shared<dae::GameObject>();
	go->Initialize();

	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x, position.y, 0);

	scene.Add(go);


	return go;
}

std::shared_ptr<dae::GameObject> AddTextGameObject(dae::Scene& scene, dae::ResourceManager& resourceManager, glm::vec2 position, std::string text, std::string fontPath, int fontSize, SDL_Color color)
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


//SDL_Scancode
std::shared_ptr<dae::GameObject> AddStandardPlayerObject(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, unsigned int playerIndex,
	glm::vec2 position, std::string imagePath, bool isKeyBoard)
{
	auto go = std::make_shared<dae::GameObject>();
	auto texture = resourceManager.LoadTexture(imagePath);
	go->Initialize();
	go->AddComponent<dae::ImageObjectComponent>()->SetTexture(texture);
	go->AddComponent<dae::ImageRenderComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x, position.y, 0);
	go->AddComponent<dae::HealthComponent>();
	go->AddComponent<dae::ScoreComponent>();

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

	return toReturnObject;
}

std::shared_ptr<dae::GameObject> AddPeterPepperPlayerObject(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, 
	unsigned int playerIndex, glm::vec2 position, std::string imagePath, bool isKeyBoard)
{
	auto go = std::make_shared<dae::GameObject>();
	auto texture = resourceManager.LoadTexture(imagePath);
	go->Initialize();
	go->AddComponent<dae::ImageObjectComponent>()->SetTexture(texture);
	go->AddComponent<dae::ImageRenderComponent>();
	go->AddComponent<dae::StateComponent>();
	go->AddComponent<dae::DirectionComponent>()->SetMovementSpeed(100);
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x, position.y, 0.1f);
	auto healthComponent = go->AddComponent<dae::HealthComponent>();
	auto playerScore = go->AddComponent<dae::ScoreComponent>();

	if (!isKeyBoard)
	{
		auto pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadUp, dae::ButtonState::Pressed }
		, std::make_unique<dae::ChangeDirectionCommand>(go));
		pMoveCommand->SetDirection(PlayerDirection::Up);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadUp, dae::ButtonState::Up }
		, std::make_unique<dae::ChangeDirectionCommand>(go));
		pMoveCommand->SetDirection(PlayerDirection::None);


		pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadDown, dae::ButtonState::Pressed }
		, std::make_unique<dae::ChangeDirectionCommand>(go));
		pMoveCommand->SetDirection(PlayerDirection::Down);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadDown, dae::ButtonState::Up }
		, std::make_unique<dae::ChangeDirectionCommand>(go));
		pMoveCommand->SetDirection(PlayerDirection::None);


		pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadRight, dae::ButtonState::Pressed }
		, std::make_unique<dae::ChangeDirectionCommand>(go));
		pMoveCommand->SetDirection(PlayerDirection::Right);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadRight, dae::ButtonState::Up }
		, std::make_unique<dae::ChangeDirectionCommand>(go));
		pMoveCommand->SetDirection(PlayerDirection::None);


		pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadLeft, dae::ButtonState::Pressed }
		, std::make_unique<dae::ChangeDirectionCommand>(go));
		pMoveCommand->SetDirection(PlayerDirection::Left);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadLeft, dae::ButtonState::Up }
		, std::make_unique<dae::ChangeDirectionCommand>(go));
		pMoveCommand->SetDirection(PlayerDirection::None);
	}
	else
	{
		auto pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_S, dae::ButtonState::Pressed }, std::make_unique<dae::ChangeDirectionCommand>(go));
		pMoveCommand->SetDirection(PlayerDirection::Down);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_S, dae::ButtonState::Up }, std::make_unique<dae::ChangeDirectionCommand>(go));
		pMoveCommand->SetDirection(PlayerDirection::None);


		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_W, dae::ButtonState::Pressed }, std::make_unique<dae::ChangeDirectionCommand>(go));
		pMoveCommand->SetDirection(PlayerDirection::Up);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_W, dae::ButtonState::Up }, std::make_unique<dae::ChangeDirectionCommand>(go));
		pMoveCommand->SetDirection(PlayerDirection::None);



		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_D, dae::ButtonState::Pressed }, std::make_unique<dae::ChangeDirectionCommand>(go));
		pMoveCommand->SetDirection(PlayerDirection::Right);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_D, dae::ButtonState::Up }, std::make_unique<dae::ChangeDirectionCommand>(go));
		pMoveCommand->SetDirection(PlayerDirection::None);


		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_A, dae::ButtonState::Pressed }, std::make_unique<dae::ChangeDirectionCommand>(go));
		pMoveCommand->SetDirection(PlayerDirection::Left);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_A, dae::ButtonState::Up }, std::make_unique<dae::ChangeDirectionCommand>(go));
		pMoveCommand->SetDirection(PlayerDirection::None);
	}

	scene.Add(go);

	//Attack component 
	auto attackComponent = go->AddComponent<dae::AttackComponent>();

	//Colision component set up
	auto worldPosition = go->GetComponent<dae::TransformComponent>()->GetWorldPosition();
	auto Texture = go->AddComponent<dae::ImageObjectComponent>()->GetTexture().get();
	auto textureSize = Texture->GetSize();
	dae::Box boxCollision{ static_cast<float>(textureSize.x), static_cast<float>(textureSize.y) };
	auto collisionComp = go->AddComponent<dae::CollisionComponent>();
	collisionComp->SetTag("Player");
	collisionComp->SetMeasurements(boxCollision);
	collisionComp->SetDebugColor({255,0,0});

	//Bind the method with what variable will call it and an X amount of parameters.
	auto bindResult = std::bind(&dae::HealthComponent::OnEnemyHit, healthComponent, std::placeholders::_1);
	collisionComp->SetOnTriggerEvent(bindResult);

	//Action Commands set up
	auto pAttackCommand = inputManager.AddControllerCommandToMap<dae::AttackCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::ButtonB, dae::ButtonState::Down }
	, std::make_unique<dae::AttackCommand>(attackComponent.get()));
	pAttackCommand = inputManager.AddKeyboardCommandToMap<dae::AttackCommand>(dae::KeyBoardInput{ SDL_SCANCODE_SPACE, dae::ButtonState::Down }, std::make_unique<dae::AttackCommand>(attackComponent.get()));


	return go;
}


std::shared_ptr<dae::GameObject> AddPlayerObjectWithScoreAndHealth(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, unsigned int playerIndex,
	glm::vec2 position, glm::vec2 healthUIPos, glm::vec2 scoreUIPos, std::string imagePath,
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

	//Lives remaining
	go = AddTextGameObject(scene, resourceManager, { healthUIPos.x, healthUIPos.y }, "This is a test", "Lingua.otf", 24, uiColor); //10, 200
	auto livesRemanainingObserver = go->AddComponent<dae::LivesRemainingUIComponent>();

	playerHealth->GetPlayerDiedEventSubject()->AddObserver(livesRemanainingObserver.get());

	//Score
	go = AddTextGameObject(scene, resourceManager, { scoreUIPos.x, scoreUIPos.y }, "Score: ", "Lingua.otf", 24, uiColor); // 10, 230
	auto scoreObserver = go->AddComponent<dae::ScoreUIComponent>();

	playerScore->GetScoreGainedSubject()->AddObserver(scoreObserver.get());
			

	return toReturnObject;
}


int main(int, char* []) {


	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}
