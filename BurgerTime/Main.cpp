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
#include "BlockingVolumeComponent.h"
#include "PlayerCollideCheckComponent.h"
#include "BurgerPieceComponent.h"
#include "EnemyCollisionCheckComponent.h"

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


void AddBlockingVolumes(dae::Scene& scene);

glm::vec2 g_windowSize{ 640,480 };

void InitTestScene();

void load()
{
	using namespace dae;
	//auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	auto& resourceManager = ResourceManager::GetInstance();
	auto font = resourceManager.LoadFont("Lingua.otf", 36);
	auto texture = resourceManager.LoadTexture("background.tga");
	auto& inputManager = InputManager::GetInstance();
	//auto& soundManager = SoundManager::GetInstance();
	auto& jsonLevelReader = JsonLevelReader::GetInstance();

	auto& lvl1Scene = jsonLevelReader.ReadAndLoadLevel("Level1.json");
	auto player = AddPeterPepperPlayerObject(lvl1Scene, resourceManager, inputManager, 0, { 200, 50 }, "MrPepperDown.png", true);
	AddBlockingVolumes(lvl1Scene);

	//Enemy																					   
	auto go = std::make_shared<GameObject>();												   
	go->Initialize();																		   
	go->GetComponent<TransformComponent>()->SetLocalPosition({ 550, 95, 0 });				   
	lvl1Scene.Add(go);
													
	auto stateComp = go->AddComponent<StateComponent>();
	stateComp->SetCurrentState(State::Walking);

	texture = resourceManager.LoadTexture("MrEggLeft.png");
	go->AddComponent<dae::ImageObjectComponent>();

	auto enemyComponent = go->AddComponent<EnemyComponent>();	
	enemyComponent->SetEnemyType(EnemyType::Egg);

	go->AddComponent<dae::ImageRenderComponent>();											   
																										   
	auto goCollisionComponent = go->AddComponent<CollisionComponent>();						   
	goCollisionComponent->SetMeasurements(static_cast<float>(texture->GetSize().x), static_cast<float>(texture->GetSize().y));
	goCollisionComponent->SetTag("Enemy");													   
	goCollisionComponent->SetDebugColor({ 255,255,0 });

	auto enemyColCheckComp = go->AddComponent<EnemyCollisionCheckComponent>();
	auto bindResult = std::bind(&dae::EnemyCollisionCheckComponent::OnGameObjectHit, enemyColCheckComp, std::placeholders::_1, std::placeholders::_2);
	goCollisionComponent->SetOnTriggerEvent(bindResult);
																						  											   
																							   
																							   
	//y: 55
	//auto go = std::make_shared<GameObject>();
	 go = std::make_shared<GameObject>();
	lvl1Scene.Add(go);
	go->Initialize();
	go->GetTransform()->SetLocalPosition({ 22, 47, 0 });

	auto mainBurgerPiece = go->AddComponent(std::make_shared<BurgerPieceComponent>(go.get(), "BunTop.png"));
	mainBurgerPiece->SetPlayer(player.get());
	//auto goCollisionComponent = go->AddComponent<CollisionComponent>();
	
	goCollisionComponent = go->AddComponent<CollisionComponent>();
	goCollisionComponent->SetTag("MainBurgerPiece");
	goCollisionComponent->SetMeasurements(static_cast<float>(mainBurgerPiece->GetTexture()->GetSize().x), static_cast<float>(mainBurgerPiece->GetTexture()->GetSize().y));
	//goCollisionComponent->SetDrawDebug(true);
	goCollisionComponent->SetOnTriggerEvent([=](CollisionComponent* otherColComp, GameObject* otherGOCol)
		{

			if (otherColComp->GetTag() != "MainBurgerPiece")
			{
				//std::cout << "BunTop!! Other tag is not MainBurgerPiece, it's: " + otherColComp->GetTag() << std::endl;
				return;
			}
			//std::cout << "Other collision is another BurgerPiece\n";


			auto otherBurgerPiece = otherGOCol->GetComponent<BurgerPieceComponent>();
			if (!otherBurgerPiece.get())
				return;

			//std::cout << "Other collision has BurgerPieceComponent\n";

			otherBurgerPiece->PushChildrenDown();

		}
	);

	//goCollisionComponent->SetMeasurements(static_cast<float>(mainBurgerPiece->GetTexture()->GetSize().x), static_cast<float>(mainBurgerPiece->GetTexture()->GetSize().y));


	go = std::make_shared<GameObject>();
	lvl1Scene.Add(go);
	go->Initialize();
	go->GetTransform()->SetLocalPosition({ 22, 82, 0 });
	//auto ownerWorldPos = go->GetTransform()->GetWorldPosition();
	mainBurgerPiece = go->AddComponent(std::make_shared<BurgerPieceComponent>(go.get(), "Lettuce.png"));
	mainBurgerPiece->SetPlayer(player.get());

	goCollisionComponent = go->AddComponent<CollisionComponent>();
	goCollisionComponent->SetTag("MainBurgerPiece");
	goCollisionComponent->SetMeasurements(static_cast<float>(mainBurgerPiece->GetTexture()->GetSize().x), static_cast<float>(mainBurgerPiece->GetTexture()->GetSize().y));
	//goCollisionComponent->SetDrawDebug(true);
	goCollisionComponent->SetOnTriggerEvent([=](CollisionComponent* otherColComp, GameObject* otherGOCol)
		{

			if (otherColComp->GetTag() != "MainBurgerPiece")
			{
				//std::cout << "Lettuce!! Other tag is not BurgerPiece, it's: " + otherColComp->GetTag() << std::endl;
				return;
			}
			//std::cout << "Other collision is another BurgerPiece\n";


			auto otherBurgerPiece = otherGOCol->GetComponent<BurgerPieceComponent>();
			if (!otherBurgerPiece.get())
				return;

			//std::cout << "Other collision has BurgerPieceComponent\n";

			otherBurgerPiece->PushChildrenDown();

		}
	);

																																											   
	
	//FPS
	go = std::make_shared<GameObject>();																																	   
	go = AddTextGameObject(lvl1Scene, resourceManager, { 0.f ,0.f }, "0", "Lingua.otf", 36);																				   
	go->AddComponent<FPSCountComponent>();


	
	////Sound
	//SoundData soundTrack{ 0,0.05f,"02_Game_Music.mp3", SoundData::SoundType::SoundTrack, false };
	//soundManager.GetSoundSystem()->NotifyQueue(soundTrack);
	//
	//SoundData soundEffect{ 1, 0.1f,"Gain_score.mp3",  SoundData::SoundType::SoundEffect,true };
	//soundManager.GetSoundSystem()->NotifyQueue(soundEffect);
}



void InitTestScene()
{
	using namespace dae;
	auto& scene = SceneManager::GetInstance().CreateScene("Sandbox");
	auto go = std::make_shared<GameObject>();
	scene.Add(go);
	go->Initialize();
	go->GetTransform()->SetLocalPosition({ 22, 47, 0 });

	auto mainBurgerPiece = go->AddComponent(std::make_shared<BurgerPieceComponent>(go.get(), "BunTop.png"));

	auto goCollisionComponent = go->AddComponent<CollisionComponent>();
	goCollisionComponent->SetTag("MainBurgerPiece");
	goCollisionComponent->SetMeasurements(static_cast<float>(mainBurgerPiece->GetTexture()->GetSize().x), static_cast<float>(mainBurgerPiece->GetTexture()->GetSize().y));
	//goCollisionComponent->SetDrawDebug(true);
	goCollisionComponent->SetOnTriggerEvent([=](CollisionComponent* otherColComp, GameObject* otherGOCol)
		{

			if (otherColComp->GetTag() != "MainBurgerPiece")
			{
				//std::cout << "BunTop!! Other tag is not MainBurgerPiece, it's: " + otherColComp->GetTag() << std::endl;
				return;
			}
			//std::cout << "Other collision is another BurgerPiece\n";


			auto otherBurgerPiece = otherGOCol->GetComponent<BurgerPieceComponent>();
			if (!otherBurgerPiece.get())
				return;

			//std::cout << "Other collision has BurgerPieceComponent\n";

			otherBurgerPiece->PushChildrenDown();

		}
	);

	//goCollisionComponent->SetMeasurements(static_cast<float>(mainBurgerPiece->GetTexture()->GetSize().x), static_cast<float>(mainBurgerPiece->GetTexture()->GetSize().y));


	go = std::make_shared<GameObject>();
	scene.Add(go);
	go->Initialize();
	go->GetTransform()->SetLocalPosition({ 22, 51, 0 });
	//auto ownerWorldPos = go->GetTransform()->GetWorldPosition();
	go->AddComponent(std::make_shared<BurgerPieceComponent>(go.get(), "Lettuce.png"));

	goCollisionComponent = go->AddComponent<CollisionComponent>();
	goCollisionComponent->SetTag("MainBurgerPiece");
	goCollisionComponent->SetMeasurements(static_cast<float>(mainBurgerPiece->GetTexture()->GetSize().x), static_cast<float>(mainBurgerPiece->GetTexture()->GetSize().y));
	//goCollisionComponent->SetDrawDebug(true);
	goCollisionComponent->SetOnTriggerEvent([=](CollisionComponent* otherColComp, GameObject* otherGOCol)
		{

			if (otherColComp->GetTag() != "MainBurgerPiece")
			{
				//std::cout << "Lettuce!! Other tag is not BurgerPiece, it's: " + otherColComp->GetTag() << std::endl;
				return;
			}
			//std::cout << "Other collision is another BurgerPiece\n";


			auto otherBurgerPiece = otherGOCol->GetComponent<BurgerPieceComponent>();
			if (!otherBurgerPiece.get())
				return;

			//std::cout << "Other collision has BurgerPieceComponent\n";

			otherBurgerPiece->PushChildrenDown();

		}
	);
}

void AddBlockingVolumes(dae::Scene& scene)
{
	auto go = AddEmptyGameObject(scene, { 0,0 });
	//CollisionComponent
	auto goCollisionComponent = go->AddComponent<dae::CollisionComponent>();
	goCollisionComponent->SetMeasurements({ 0,0, 20, g_windowSize.y });
	auto collisionSize = goCollisionComponent->GetMeasurements();
	goCollisionComponent->SetTag("BlockingVolumeLeft");
	goCollisionComponent->SetDrawDebug(true);

	//BlockingVolumeComponent
	auto bvComp = go->AddComponent<dae::BlockingVolumeComponent>();
	bvComp->SetWindowSize(g_windowSize);
	
	auto bindResult = std::bind(&dae::BlockingVolumeComponent::OnGameObjectHit, bvComp, std::placeholders::_1, std::placeholders::_2);
	goCollisionComponent->SetOnTriggerEvent(bindResult);




	go = AddEmptyGameObject(scene, { g_windowSize.x - collisionSize.width, 0.f });
	//CollisionComponent
	goCollisionComponent = go->AddComponent<dae::CollisionComponent>();
	goCollisionComponent->SetMeasurements({ 0,0,20, g_windowSize.y });
	collisionSize =  goCollisionComponent->GetMeasurements();
	goCollisionComponent->SetTag("BlockingVolumeRight");	
	goCollisionComponent->SetDrawDebug(true);

	//BlockingVolumeComponent
	bvComp = go->AddComponent<dae::BlockingVolumeComponent>();
	bvComp->SetWindowSize(g_windowSize);


	bindResult = std::bind(&dae::BlockingVolumeComponent::OnGameObjectHit, bvComp, std::placeholders::_1, std::placeholders::_2);
	goCollisionComponent->SetOnTriggerEvent(bindResult);

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
	auto goRaw = go.get();
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
		, std::make_unique<dae::MoveCommand>(goRaw));

		pMoveCommand->SetDirection({ 0,-1 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::MoveCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadDown, dae::ButtonState::Pressed }
		, std::make_unique<dae::MoveCommand>(goRaw));

		pMoveCommand->SetDirection({ 0,1 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::MoveCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadRight, dae::ButtonState::Pressed }
		, std::make_unique<dae::MoveCommand>(goRaw));

		pMoveCommand->SetDirection({ 1,0 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::MoveCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadLeft, dae::ButtonState::Pressed }
		, std::make_unique<dae::MoveCommand>(goRaw));

		pMoveCommand->SetDirection({ -1,0 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

	}
	else
	{
		auto pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::MoveCommand>(dae::KeyBoardInput{ SDL_SCANCODE_S, dae::ButtonState::Pressed }, std::make_unique<dae::MoveCommand>(goRaw));
		pMoveCommand->SetDirection({ 0,1 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::MoveCommand>(dae::KeyBoardInput{ SDL_SCANCODE_W, dae::ButtonState::Pressed }, std::make_unique<dae::MoveCommand>(goRaw));
		pMoveCommand->SetDirection({ 0,-1 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::MoveCommand>(dae::KeyBoardInput{ SDL_SCANCODE_D, dae::ButtonState::Pressed }, std::make_unique<dae::MoveCommand>(goRaw));
		pMoveCommand->SetDirection({ 1,0 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::MoveCommand>(dae::KeyBoardInput{ SDL_SCANCODE_A, dae::ButtonState::Pressed }, std::make_unique<dae::MoveCommand>(goRaw));
		pMoveCommand->SetDirection({ -1,0 });
		pMoveCommand->SetMovementSpeed(movementSpeed);
	}

	auto& toReturnObject = go;
	scene.Add(go);

	return toReturnObject;
}

std::shared_ptr<dae::GameObject> AddPeterPepperPlayerObject(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, 
	unsigned int playerIndex, glm::vec2 position, std::string imagePath, bool isKeyBoard)
{
	auto go = std::make_shared<dae::GameObject>();
	auto goRaw = go.get();
	auto texture = resourceManager.LoadTexture(imagePath);
	go->Initialize();

	auto playerHealth = go->AddComponent<dae::HealthComponent>();

	auto imageObjComp = go->AddComponent<dae::ImageObjectComponent>();
	imageObjComp->SetTexture(texture);
	go->AddComponent<dae::ImageRenderComponent>();

	auto playerDirectionComp = go->AddComponent<dae::DirectionComponent>();
	playerDirectionComp->SetMovementSpeed(100);
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x, position.y, 0.1f);
	auto playerScore = go->AddComponent<dae::ScoreComponent>();


	if (!isKeyBoard)
	{
		auto pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadUp, dae::ButtonState::Pressed }
		, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(PlayerDirection::Up);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadUp, dae::ButtonState::Up }
		, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(PlayerDirection::None);


		pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadDown, dae::ButtonState::Pressed }
		, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(PlayerDirection::Down);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadDown, dae::ButtonState::Up }
		, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(PlayerDirection::None);


		pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadRight, dae::ButtonState::Pressed }
		, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(PlayerDirection::Right);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadRight, dae::ButtonState::Up }
		, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(PlayerDirection::None);


		pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadLeft, dae::ButtonState::Pressed }
		, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(PlayerDirection::Left);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadLeft, dae::ButtonState::Up }
		, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(PlayerDirection::None);
	}
	else
	{
		auto pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_S, dae::ButtonState::Pressed }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(PlayerDirection::Down);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_S, dae::ButtonState::Up }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(PlayerDirection::None);


		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_W, dae::ButtonState::Pressed }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(PlayerDirection::Up);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_W, dae::ButtonState::Up }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(PlayerDirection::None);



		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_D, dae::ButtonState::Pressed }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(PlayerDirection::Right);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_D, dae::ButtonState::Up }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(PlayerDirection::None);


		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_A, dae::ButtonState::Pressed }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(PlayerDirection::Left);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_A, dae::ButtonState::Up }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(PlayerDirection::None);
	}

	scene.Add(go);
	

	//Colision component set up
	auto textureSize = texture->GetSize();
	auto boxWidth = static_cast<float>(textureSize.x);
	auto boxHeight = static_cast<float>(textureSize.y);
	auto collisionComp = go->AddComponent<dae::CollisionComponent>();
	collisionComp->SetTag("Player");
	collisionComp->SetMeasurements(boxWidth, boxHeight);
	collisionComp->SetDebugColor({255,0,0});
	//collisionComp->SetDrawDebug(true);
	
	auto bindResult = std::bind(&dae::HealthComponent::OnEnemyHit, playerHealth, std::placeholders::_1, std::placeholders::_2);
	collisionComp->SetOnTriggerEvent(bindResult);

	auto pToReturn = go;

	//Player Score UI
	go = AddTextGameObject(scene, resourceManager, { 10, 230 }, "Score: ", "Lingua.otf", 24, { 255,255,0 }); // 10, 230		 
	auto scoreObserver = go->AddComponent<dae::ScoreUIComponent>();															 
																															 
	playerScore->GetScoreGainedSubject()->AddObserver(scoreObserver.get());

	//Player Health UI
	go = AddTextGameObject(scene, resourceManager, { 10, 250 }, "Score: ", "Lingua.otf", 24, { 255,255,0 }); // 10, 230
	auto healthObserver = go->AddComponent<dae::LivesRemainingUIComponent>();

	playerHealth->GetPlayerDiedEventSubject()->AddObserver(healthObserver.get());




//Child
	auto childGO = std::make_shared <dae::GameObject>();
	childGO->Initialize();
	
	childGO->SetParent(pToReturn.get(), false);
	//Player Collision Specific Compontent
	const auto halfTextureWidth = textureSize.x / 2.f;

	auto childStateComp = childGO->AddComponent<dae::StateComponent>();
	playerDirectionComp->SetDirectionStateComponent(childStateComp.get());

	auto playColComp = childGO->AddComponent<dae::PlayerCollideCheckComponent>();
	playColComp->SetParentImageObjectComponent(imageObjComp.get());


	auto childColComp = childGO->AddComponent<dae::CollisionComponent>();
	childColComp->SetMeasurements(dae::Box{ 0,0, halfTextureWidth, static_cast<float>(textureSize.y) });
	childColComp->SetDebugColor({ 255,255,0 });
	childColComp->SetTag("PlayerLevelCheck");
	childColComp->SetDrawDebug(true);


	//Attack component 
	auto attackComponent = childGO->AddComponent<dae::AttackComponent>();
	//Action Commands set up
	auto pAttackCommand = inputManager.AddControllerCommandToMap<dae::AttackCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::ButtonB, dae::ButtonState::Down }
	, std::make_unique<dae::AttackCommand>(attackComponent.get()));
	pAttackCommand = inputManager.AddKeyboardCommandToMap<dae::AttackCommand>(dae::KeyBoardInput{ SDL_SCANCODE_SPACE, dae::ButtonState::Down }, std::make_unique<dae::AttackCommand>(attackComponent.get()));



	//Bind the method with what variable will call it and an X amount of parameters.
	auto childBindResult = std::bind(&dae::PlayerCollideCheckComponent::OnGameObjectCollision, playColComp, std::placeholders::_1, std::placeholders::_2);
	childColComp->SetOnTriggerEvent(childBindResult);

	
	childGO->GetTransform()->SetLocalPosition(halfTextureWidth / 2.f, 0, 0);



	return pToReturn;
}


std::shared_ptr<dae::GameObject> AddPlayerObjectWithScoreAndHealth(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, unsigned int playerIndex,
	glm::vec2 position, glm::vec2 healthUIPos, glm::vec2 scoreUIPos, std::string imagePath,
	SDL_Color uiColor, bool isKeyBoard)
{
	auto go = std::make_shared<dae::GameObject>();
	auto goRaw = go.get();
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
		, std::make_unique<dae::MoveCommand>(goRaw));

		pMoveCommand->SetDirection({ 0,-1 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::MoveCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadDown, dae::ButtonState::Pressed }
		, std::make_unique<dae::MoveCommand>(goRaw));

		pMoveCommand->SetDirection({ 0,1 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::MoveCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadRight, dae::ButtonState::Pressed }
		, std::make_unique<dae::MoveCommand>(goRaw));

		pMoveCommand->SetDirection({ 1,0 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::MoveCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadLeft, dae::ButtonState::Pressed }
		, std::make_unique<dae::MoveCommand>(goRaw));

		pMoveCommand->SetDirection({ -1,0 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

	}
	else
	{
		auto pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::MoveCommand>(dae::KeyBoardInput{ SDL_SCANCODE_S, dae::ButtonState::Pressed }, std::make_unique<dae::MoveCommand>(goRaw));
		pMoveCommand->SetDirection({ 0,1 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::MoveCommand>(dae::KeyBoardInput{ SDL_SCANCODE_W, dae::ButtonState::Pressed }, std::make_unique<dae::MoveCommand>(goRaw));
		pMoveCommand->SetDirection({ 0,-1 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::MoveCommand>(dae::KeyBoardInput{ SDL_SCANCODE_D, dae::ButtonState::Pressed }, std::make_unique<dae::MoveCommand>(goRaw));
		pMoveCommand->SetDirection({ 1,0 });
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::MoveCommand>(dae::KeyBoardInput{ SDL_SCANCODE_A, dae::ButtonState::Pressed }, std::make_unique<dae::MoveCommand>(goRaw));
		pMoveCommand->SetDirection({ -1,0 });
		pMoveCommand->SetMovementSpeed(movementSpeed);
	}

	auto& toReturnObject = go;
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


	dae::Minigin engine("../Data/", g_windowSize.x, g_windowSize.y);
	engine.Run(load); //Put back to load later, testscene for sandbox
	
	return 0;
}
