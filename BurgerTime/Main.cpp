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
#include "HighscoreManager.h"

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
#include "BurgerPlateComponent.h"
#include "PepperUsesUIComponent.h"
#include "MenuButtonComponent.h"
#include "ButtonIndexManagerComponent.h"
#include "CheckLevelFinishedComponent.h"
#include "HighscoreDisplayComponent.h"

//Commands
#include "Controller.h"
#include "Command.h"
#include "MoveCommand.h"
#include "DieCommand.h"
#include "GainScoreCommand.h"
#include "ChangeDirectionCommand.h"
#include "AttackCommand.h"
#include "ChangeButtonIndexCommand.h"
#include "MenuButtonPressCommand.h"
#include "ReturnToMainMenuCommand.h"
#include "LevelSkipCommand.h"

//Extras
#include "Texture2D.h"
#include "JsonFileReader.h"
#include "Structs.h"

std::shared_ptr<dae::GameObject> AddEmptyGameObject(dae::Scene& scene, glm::vec2 position);

std::shared_ptr<dae::GameObject> AddTextGameObject(dae::Scene& scene, dae::ResourceManager& resourceManager, glm::vec2 position, std::string text, std::string fontPath, int fontSize, SDL_Color color = { 255,255,255 });

std::shared_ptr<dae::GameObject> AddImageGameObject(dae::Scene& scene, dae::ResourceManager& resourceManager, glm::vec2 position, std::string imagePath);

std::shared_ptr<dae::GameObject> AddStandardPlayerObject(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, unsigned int playerIndex,
	glm::vec2 position, std::string imagePath, bool canUseKeyboard);

std::shared_ptr<dae::GameObject> AddPeterPepperPlayerObject(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, dae::GameState gameState, 
	dae::CheckLevelFinishedComponent* pLevelFinishComp, unsigned int playerIndex, glm::vec2 position, glm::vec2 healthUIPos, glm::vec2 scoreUIPos, glm::vec2 pepperUIPos, std::string imagePath, bool canUseKeyboard, bool isPlayerTwo = false, SDL_Color UIColor = { 255,255,255 });


std::shared_ptr<dae::GameObject> AddPlayerObjectWithScoreAndHealth(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, unsigned int playerIndex,
	glm::vec2 position, glm::vec2 healthUIPos, glm::vec2 scoreUIPos, std::string imagePath,
	SDL_Color uiColor, bool canUseKeyboard);


void AddBlockingVolumes(dae::Scene& scene);

void AddBurgerPieces(dae::Scene& scene, dae::GameObject* player, dae::CheckLevelFinishedComponent* pCheckLevelFinishedComp);
void AddBurgerPiece(dae::Scene& scene, dae::GameObject* player, dae::CheckLevelFinishedComponent* pCheckLevelFinishedComp, int index, glm::vec2 position, const std::string& imagePath);

void AddBurgerPlates(dae::Scene& scene, std::shared_ptr<dae::Texture2D> texture);
void AddBurgerPlate(dae::Scene& scene, std::shared_ptr<dae::Texture2D> texture, glm::vec2 pos);


void MakeMainMenuScene(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, dae::SceneManager& sceneManager);
void LoadLevel(dae::JsonFileReader& levelReader, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, const std::string& fileName, dae::GameState gameState,bool canUseKeyboard = false);
void AddAdditionalContent(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, dae::GameState gameState, dae::CheckLevelFinishedComponent* pLevelFinishComp);

void MakeEnemyByType(dae::Scene& scene, dae::EnemyType enemyType, glm::vec2 enemyPos);


glm::vec2 g_windowSize{ 640,480 };

void InitTestScene();

void load()
{
	srand(static_cast<unsigned int>(time(nullptr)));


	using namespace dae;
	auto& sceneManager = SceneManager::GetInstance();
	auto& resourceManager = ResourceManager::GetInstance();
	auto font = resourceManager.LoadFont("Lingua.otf", 36);
	auto texture = resourceManager.LoadTexture("background.tga");
	auto& inputManager = InputManager::GetInstance();
	auto& soundManager = SoundManager::GetInstance();
	//auto& jsonLevelReader = JsonLevelReader::GetInstance();


	auto& highScoreMenu = sceneManager.CreateScene("Highscore Scene");

	auto go = AddTextGameObject(highScoreMenu, resourceManager, {(g_windowSize.x / 2.f) - 100.f, 50.f}, "Highscore", "Lingua.otf", 36);
	go = std::make_shared<GameObject>();
	go->Initialize();
	auto pHighscoreDispComp = go->AddComponent<HighscoreDisplayComponent>();
	highScoreMenu.Add(go);

	HighscoreManager::GetInstance().SetHighScoreDisplayComp(pHighscoreDispComp.get());

	auto& mainMenuScene = sceneManager.CreateScene("Main Menu");
	MakeMainMenuScene(mainMenuScene, resourceManager, inputManager, sceneManager);



	inputManager.AddControllerCommandToMap<dae::ReturnToMainMenuCommand>(dae::ControllerInput{ 0, dae::Controller::ControllerButton::ButtonB, dae::ButtonState::Down, highScoreMenu.GetName() }
	, std::make_unique<dae::ReturnToMainMenuCommand>());

	inputManager.AddKeyboardCommandToMap<dae::ReturnToMainMenuCommand>(dae::KeyBoardInput{ SDL_SCANCODE_RETURN, dae::ButtonState::Up, highScoreMenu.GetName() }, std::make_unique<dae::ReturnToMainMenuCommand>());
	//highScoreMenu.Add(go);




	sceneManager.SetActiveSceneByName(mainMenuScene.GetName());

	////Sound
	SoundData soundTrack{ 0,0.05f,"02_Game_Music.mp3", SoundData::SoundType::SoundTrack, false };
	soundManager.GetSoundSystem()->NotifyQueue(soundTrack);
	
	SoundData soundEffect{ 1, 0.2f,"Gain_score.mp3",  SoundData::SoundType::SoundEffect,true };
	soundManager.GetSoundSystem()->NotifyQueue(soundEffect);

	soundEffect = SoundData{ 2, 0.2f,"03_Player_Hurt.mp3",  SoundData::SoundType::SoundEffect,true };
	soundManager.GetSoundSystem()->NotifyQueue(soundEffect);
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
	collisionSize = goCollisionComponent->GetMeasurements();
	goCollisionComponent->SetTag("BlockingVolumeRight");
	goCollisionComponent->SetDrawDebug(true);

	//BlockingVolumeComponent
	bvComp = go->AddComponent<dae::BlockingVolumeComponent>();
	bvComp->SetWindowSize(g_windowSize);


	bindResult = std::bind(&dae::BlockingVolumeComponent::OnGameObjectHit, bvComp, std::placeholders::_1, std::placeholders::_2);
	goCollisionComponent->SetOnTriggerEvent(bindResult);

}

void AddBurgerPieces(dae::Scene& scene, dae::GameObject* player, dae::CheckLevelFinishedComponent* pCheckLevelFinishedComp)
{
	using namespace dae;

	if (strcmp(scene.GetName().c_str(), "Level 1") == 0)
	{
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 0, { 295,44 }, "BunTop.png");
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 1, { 295,83 }, "Lettuce.png");
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 2, { 295,122 }, "Patty.png");
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 3, { 295,161 }, "BunBottom.png");
	}
	else if (strcmp(scene.GetName().c_str(), "Level 2") == 0)
	{
		//x 270
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 0, { 270,44 }, "BunTop.png");
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 1, { 270,83 }, "Lettuce.png");
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 2, { 270,122 }, "Patty.png");
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 3, { 270,161 }, "Cheese.png");
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 4, { 270,200 }, "BunBottom.png");
		 
		//x 350
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 0, { 350,44 }, "BunTop.png");
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 1, { 350,83 }, "Lettuce.png");
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 2, { 350,122 }, "Patty.png");
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 3, { 350,161 }, "Cheese.png");
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 4, { 350,200 }, "BunBottom.png");
	}
	else if (strcmp(scene.GetName().c_str(), "Level 3") == 0)
	{
		//x 240
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 0, { 240,44 }, "BunTop.png");
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 1, { 240,83 }, "Lettuce.png");
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 2, { 240,122 }, "Patty.png");
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 3, { 240,161 }, "Cheese.png");
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 4, { 240,200 }, "BunBottom.png");
		//x 350
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 0, { 350,44 }, "BunTop.png");
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 1, { 350,83 }, "Lettuce.png");
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 2, { 350,122 }, "Patty.png");
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 3, { 350,161 }, "Cheese.png");
		AddBurgerPiece(scene, player, pCheckLevelFinishedComp, 4, { 350,200 }, "BunBottom.png");

	}
}
void AddBurgerPiece(dae::Scene& scene, dae::GameObject* player, dae::CheckLevelFinishedComponent* pCheckLevelFinishedComp, int index, glm::vec2 position, const std::string& imagePath)
{
	using namespace dae;
	//y: 55
	//auto go = std::make_shared<GameObject>();
	auto go = std::make_shared<GameObject>();
	scene.Add(go);
	go->Initialize();
	go->GetTransform()->SetLocalPosition({ position.x, position.y, 0 });

	auto mainBurgerPiece = go->AddComponent(std::make_shared<BurgerPieceComponent>(go.get(), imagePath));
	mainBurgerPiece->SetPlayer(player);
	mainBurgerPiece->SetIndex(index);

	mainBurgerPiece->GetBurgerPieceFinishedSubject()->AddObserver(pCheckLevelFinishedComp);
	//Subject observer set up
	pCheckLevelFinishedComp->IncrementUncompletedPieces();
}

void AddBurgerPlates(dae::Scene& scene, std::shared_ptr<dae::Texture2D> texture)
{
	using namespace dae; 
	if (strcmp(scene.GetName().c_str(), "Level 1") == 0)
	{
		AddBurgerPlate(scene, texture, { 295, 250 });
	}
	else if (strcmp(scene.GetName().c_str(), "Level 2") == 0)
	{
		//x 270
		AddBurgerPlate(scene, texture, { 270, 300 });
		//x 350
		AddBurgerPlate(scene, texture, { 350, 300 });
	}
	else if (strcmp(scene.GetName().c_str(), "Level 3") == 0)
	{
		//x 240
		AddBurgerPlate(scene, texture, { 240, 350 });
		//x 350
		AddBurgerPlate(scene, texture, { 350, 350 });
	}

}

void AddBurgerPlate(dae::Scene& scene, std::shared_ptr<dae::Texture2D> texture, glm::vec2 pos)
{
	using namespace dae;

	auto go = std::make_shared<GameObject>();
	go->Initialize();
	go->GetTransform()->SetLocalPosition({ pos.x, pos.y, 0 });
	


	scene.Add(go);

	auto burgerPlateComp = go->AddComponent<BurgerPlateComponent>();
	auto imageObjComp = go->AddComponent<ImageObjectComponent>();
	imageObjComp->SetTexture(texture);
	auto textureSize = imageObjComp->GetTexture()->GetSize();

	go->AddComponent<ImageRenderComponent>();

	auto colComp = go->AddComponent<CollisionComponent>();
	colComp->SetMeasurements(static_cast<float>(textureSize.x), static_cast<float>(textureSize.y));
	colComp->SetTag("BurgerPlate");

	auto bindResult = std::bind(&dae::BurgerPlateComponent::OnBurgerPieceHit, burgerPlateComp, std::placeholders::_1, std::placeholders::_2);
	colComp->SetOnTriggerEvent(bindResult);
}

void MakeMainMenuScene(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, dae::SceneManager& sceneManager)
{
	using namespace dae;
	std::vector<MenuButtonComponent*> menuButtons;
	auto& jsonLevelReader = JsonFileReader::GetInstance();

	auto singlePlayerButton = AddTextGameObject(scene, resourceManager, { (g_windowSize.x / 2.f) - 100.f, 150.f }, "Singleplayer", "Lingua.otf", 36);


	auto buttonComp = singlePlayerButton->AddComponent(std::make_shared<MenuButtonComponent>(singlePlayerButton.get(),
		[&]()
		{
			GameStateLookUp::g_CurrentGameState = GameState::SinglePlayer;
			LoadLevel(jsonLevelReader, resourceManager, inputManager, "Level1.json", GameStateLookUp::g_CurrentGameState, true);
			LoadLevel(jsonLevelReader, resourceManager, inputManager, "Level2.json", GameStateLookUp::g_CurrentGameState, true);
			LoadLevel(jsonLevelReader, resourceManager, inputManager, "Level3.json", GameStateLookUp::g_CurrentGameState, true);
			sceneManager.SetActiveSceneByName("Level 1");
		},
		"SinglePlayer", 0));
	buttonComp->SetSelected(true);



	auto coopButton = AddTextGameObject(scene, resourceManager, { (g_windowSize.x / 2.f) - 100.f, 200.f }, "Co-op", "Lingua.otf", 36);

	auto coopbuttonComp = coopButton->AddComponent(std::make_shared<MenuButtonComponent>(coopButton.get(),
		[&]()
		{
			GameStateLookUp::g_CurrentGameState = GameState::Coop;
			LoadLevel(jsonLevelReader, resourceManager, inputManager, "Level1.json", GameStateLookUp::g_CurrentGameState, true);
			LoadLevel(jsonLevelReader, resourceManager, inputManager, "Level2.json", GameStateLookUp::g_CurrentGameState, true);
			LoadLevel(jsonLevelReader, resourceManager, inputManager, "Level3.json", GameStateLookUp::g_CurrentGameState, true);
			sceneManager.SetActiveSceneByName("Level 1");
		}, "Co-op", 1));




	auto versusButton = AddTextGameObject(scene, resourceManager, { (g_windowSize.x / 2.f) - 100.f, 250.f }, "Versus", "Lingua.otf", 36);

	auto versusbuttonComp = versusButton->AddComponent(std::make_shared<MenuButtonComponent>(versusButton.get(),
		[&]()
		{
			GameStateLookUp::g_CurrentGameState = GameState::Versus;
			LoadLevel(jsonLevelReader, resourceManager, inputManager, "Level1.json", GameStateLookUp::g_CurrentGameState, true);
			LoadLevel(jsonLevelReader, resourceManager, inputManager, "Level2.json", GameStateLookUp::g_CurrentGameState, true);
			LoadLevel(jsonLevelReader, resourceManager, inputManager, "Level3.json", GameStateLookUp::g_CurrentGameState, true);
			sceneManager.SetActiveSceneByName("Level 1");
		}, "Versus", 2));


	menuButtons.emplace_back(buttonComp.get());
	menuButtons.emplace_back(coopbuttonComp.get());
	menuButtons.emplace_back(versusbuttonComp.get());


	auto pButtonIdxManager = AddEmptyGameObject(scene, { 0,0 });
	auto pButtonIdxManagerComp = pButtonIdxManager->AddComponent(std::make_shared<ButtonIndexManagerComponent>(pButtonIdxManager.get(), menuButtons));

#pragma region Button_Command_Setup

	inputManager.AddControllerCommandToMap<dae::ChangeButtonIndexCommand>(dae::ControllerInput{ 0, dae::Controller::ControllerButton::DPadUp, dae::ButtonState::Down, scene.GetName() }
	, std::make_unique<dae::ChangeButtonIndexCommand>(pButtonIdxManagerComp.get(), false));

	inputManager.AddControllerCommandToMap<dae::ChangeButtonIndexCommand>(dae::ControllerInput{ 0, dae::Controller::ControllerButton::DPadDown, dae::ButtonState::Down, scene.GetName() }
	, std::make_unique<dae::ChangeButtonIndexCommand>(pButtonIdxManagerComp.get(), true));


	inputManager.AddKeyboardCommandToMap<dae::ChangeButtonIndexCommand>(dae::KeyBoardInput{ SDL_SCANCODE_W, dae::ButtonState::Down, scene.GetName() }
	, std::make_unique<dae::ChangeButtonIndexCommand>(pButtonIdxManagerComp.get(), false));

	inputManager.AddKeyboardCommandToMap<dae::ChangeButtonIndexCommand>(dae::KeyBoardInput{ SDL_SCANCODE_S, dae::ButtonState::Down, scene.GetName() }
	, std::make_unique<dae::ChangeButtonIndexCommand>(pButtonIdxManagerComp.get(), true));



	inputManager.AddKeyboardCommandToMap<dae::MenuButtonPressCommand>(dae::KeyBoardInput{ SDL_SCANCODE_RETURN, dae::ButtonState::Down, scene.GetName() }
	, std::make_unique<dae::MenuButtonPressCommand>(pButtonIdxManagerComp.get()));
#pragma endregion Button_Command_Setup


}

void LoadLevel(dae::JsonFileReader& levelReader, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, const std::string& fileName, dae::GameState gameState, bool canUseKeyboard)
{
	using namespace dae;
	auto& lvlScene = levelReader.ReadAndLoadLevel(fileName);
	AddBlockingVolumes(lvlScene);

	auto go = std::make_shared<GameObject>();
	go->Initialize();
	auto pCheckLevelFinishedComp = go->AddComponent<CheckLevelFinishedComponent>();
	lvlScene.Add(go);

	inputManager.AddKeyboardCommandToMap<dae::LevelSkipCommand>(dae::KeyBoardInput{ SDL_SCANCODE_F1, dae::ButtonState::Down, lvlScene.GetName()}, std::make_unique<dae::LevelSkipCommand>(pCheckLevelFinishedComp.get()));
	

	//pCheckLevelFinishedComp->SetScoreUIComponent();
	std::shared_ptr<GameObject> player;
	switch (gameState)
	{
	case GameState::SinglePlayer:
		//PLAYER ONE
		player = AddPeterPepperPlayerObject(lvlScene, resourceManager, inputManager, gameState, pCheckLevelFinishedComp.get(), 0, { 200, 22 }, { 10, g_windowSize.y - 60.f }, { 10, g_windowSize.y - 20.f }, { 10, g_windowSize.y - 40.f }, "MrPepperDown.png", canUseKeyboard, false, { 255,0,0 });
		break;
	case GameState::Versus:
	case GameState::Coop:
		//PLAYER ONE
		player = AddPeterPepperPlayerObject(lvlScene, resourceManager, inputManager, gameState, pCheckLevelFinishedComp.get(), 1, { 200, 22 }, { 10, g_windowSize.y - 60.f }, { 10, g_windowSize.y - 20.f }, { 10, g_windowSize.y - 40.f }, "MrPepperDown.png", canUseKeyboard, false, { 255,0,0 });
		break;
	default:
		break;
	}


	
	
	//These will be placed on the same spot in every scene.
	AddBurgerPieces(lvlScene, player.get(), pCheckLevelFinishedComp.get());

	auto texture = resourceManager.LoadTexture("Plate.png");
	AddBurgerPlates(lvlScene, texture);


	AddAdditionalContent(lvlScene, resourceManager, inputManager, gameState, pCheckLevelFinishedComp.get());

}


void AddAdditionalContent(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, dae::GameState gameState, dae::CheckLevelFinishedComponent* pLevelFinishComp)
{
	using namespace dae;
	if (gameState != GameState::Versus)
	{

		MakeEnemyByType(scene, EnemyType::Pickle, { 170, 60 });
		MakeEnemyByType(scene, EnemyType::Bean, { 325, 60 });

		MakeEnemyByType(scene, EnemyType::Bean, { 250, 105 });
		MakeEnemyByType(scene, EnemyType::Egg, { 350, 105 });


		MakeEnemyByType(scene, EnemyType::Pickle, { 250, 145 });
		MakeEnemyByType(scene, EnemyType::Bean, { 50, 145 });

		if (strcmp(scene.GetName().c_str(), "Level 2") == 0)
		{
			MakeEnemyByType(scene, EnemyType::Pickle, { 180, 185 });
			MakeEnemyByType(scene, EnemyType::Bean, { 250, 185 });

		}
		else if (strcmp(scene.GetName().c_str(), "Level 3") == 0)
		{
			MakeEnemyByType(scene, EnemyType::Bean, { 180, 185 });
			MakeEnemyByType(scene, EnemyType::Bean, { 250, 185 });

			MakeEnemyByType(scene, EnemyType::Egg, { 400, 225 });
			MakeEnemyByType(scene, EnemyType::Bean, { 30, 225 });

			MakeEnemyByType(scene, EnemyType::Pickle, { 126, 260 });
			MakeEnemyByType(scene, EnemyType::Bean, { 345, 260 });
		}


		if (gameState == GameState::Coop)
		{
			//PLAYER 2
			AddPeterPepperPlayerObject(scene, resourceManager, inputManager, gameState, pLevelFinishComp, 0, { 250, 22 }, { 10, g_windowSize.y - 100.f }, { 10, g_windowSize.y - 100.f }, { 10, g_windowSize.y - 80.f }, "MsSaltDown.png", false, true, { 0,0,255 });
		}

	}
	else if (gameState == GameState::Versus)
	{
		//PLAYER 2
		AddPeterPepperPlayerObject(scene, resourceManager, inputManager, gameState, pLevelFinishComp, 0, { 250, 22 }, { 10, g_windowSize.y - 100.f }, { 10, g_windowSize.y - 100.f }, { 10, g_windowSize.y - 80.f }, "MrBeanDown.png", false, true, { 0,0,255 });
	}

}

void MakeEnemyByType(dae::Scene& scene, dae::EnemyType enemyType, glm::vec2 enemyPos)
{
	using namespace dae;
	//Enemy																					   
	auto go = std::make_shared<GameObject>();
	go->Initialize();
	go->GetComponent<TransformComponent>()->SetLocalPosition({ enemyPos.x, enemyPos.y , 0 });
	scene.Add(go);

	auto stateComp = go->AddComponent<StateComponent>();
	stateComp->SetCurrentState(State::Walking);

	//texture = resourceManager.LoadTexture("MrEggLeft.png");
	auto imgObjComp = go->AddComponent<dae::ImageObjectComponent>();


	auto enemyComponent = go->AddComponent<EnemyComponent>();
	enemyComponent->SetEnemyType(enemyType);
	if (rand() % 2 == 0)
	{
		enemyComponent->SetMovementSpeed(enemyComponent->GetMovementSpeed() * -1.f);
	}
	auto texture = imgObjComp->GetTexture();
	go->AddComponent<dae::ImageRenderComponent>();

	auto goCollisionComponent = go->AddComponent<CollisionComponent>();
	goCollisionComponent->SetMeasurements(static_cast<float>(texture->GetSize().x), static_cast<float>(texture->GetSize().y));
	goCollisionComponent->SetTag("Enemy");
	goCollisionComponent->SetDebugColor({ 255,255,0 });

	auto enemyColCheckComp = go->AddComponent<EnemyCollisionCheckComponent>();
	auto bindResult = std::bind(&dae::EnemyCollisionCheckComponent::OnGameObjectHit, enemyColCheckComp, std::placeholders::_1, std::placeholders::_2);
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

std::shared_ptr<dae::GameObject> AddStandardPlayerObject(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, unsigned int playerIndex,
	glm::vec2 position, std::string imagePath, bool canUseKeyboard)
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

	auto sceneName = scene.GetName();
#pragma region Command_Setup
	if (canUseKeyboard)
	{
		auto pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_S, dae::ButtonState::Pressed, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::Down);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_S, dae::ButtonState::Up, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::None);


		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_W, dae::ButtonState::Pressed, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::Up);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_W, dae::ButtonState::Up, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::None);



		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_D, dae::ButtonState::Pressed, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::Right);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_D, dae::ButtonState::Up, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::None);


		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_A, dae::ButtonState::Pressed, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::Left);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_A, dae::ButtonState::Up, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::None);

	}

	auto pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadUp, dae::ButtonState::Pressed, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::Up);

	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadUp, dae::ButtonState::Up, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::None);


	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadDown, dae::ButtonState::Pressed, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::Down);

	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadDown, dae::ButtonState::Up, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::None);


	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadRight, dae::ButtonState::Pressed, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::Right);

	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadRight, dae::ButtonState::Up, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::None);


	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadLeft, dae::ButtonState::Pressed, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::Left);

	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadLeft, dae::ButtonState::Up, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::None);


#pragma endregion Command_Setup

	auto& toReturnObject = go;
	scene.Add(go);

	return toReturnObject;
}

std::shared_ptr<dae::GameObject> AddPeterPepperPlayerObject(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, dae::GameState gameState,
	dae::CheckLevelFinishedComponent* pLevelFinishComp, unsigned int playerIndex, glm::vec2 position, glm::vec2 healthUIPos, glm::vec2 scoreUIPos, glm::vec2 pepperUIPos,
	std::string imagePath, bool canUseKeyboard, bool isPlayerTwo, SDL_Color UIColor)
{
#pragma region MakePlayerPtr
	auto go = std::make_shared<dae::GameObject>();
	auto goRaw = go.get();
	go->Initialize();
#pragma endregion MakePlayerPtr

#pragma region Player_Component_Setup
	auto texture = resourceManager.LoadTexture(imagePath);
	auto playerHealth = go->AddComponent<dae::HealthComponent>();

	auto imageObjComp = go->AddComponent<dae::ImageObjectComponent>();
	imageObjComp->SetTexture(texture);
	go->AddComponent<dae::ImageRenderComponent>();

	auto playerDirectionComp = go->AddComponent(std::make_shared< dae::DirectionComponent>(go.get(), isPlayerTwo, gameState));
	playerDirectionComp->SetMovementSpeed(100);
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x, position.y, 0.1f);
	auto playerScore = go->AddComponent<dae::ScoreComponent>();

	scene.Add(go);


	//Colision component set up
	auto textureSize = texture->GetSize();
	auto boxWidth = static_cast<float>(textureSize.x);
	auto boxHeight = static_cast<float>(textureSize.y);
	auto collisionComp = go->AddComponent<dae::CollisionComponent>();
	collisionComp->SetTag("Player");

	if (gameState == dae::GameState::Versus && isPlayerTwo)
		collisionComp->SetTag("Enemy");

	collisionComp->SetMeasurements(boxWidth, boxHeight);
	collisionComp->SetDebugColor({ 255,0,0 });
	//collisionComp->SetDrawDebug(true);

	auto bindResult = std::bind(&dae::HealthComponent::OnEnemyHit, playerHealth, std::placeholders::_1, std::placeholders::_2);
	collisionComp->SetOnTriggerEvent(bindResult);

	auto pToReturn = go;

	if (!(gameState == dae::GameState::Versus && isPlayerTwo))
	{
		if (!isPlayerTwo)
		{
			//Player Score UI
			go = AddTextGameObject(scene, resourceManager, scoreUIPos, "Score: ", "Lingua.otf", 16); // 10, 230		 
			auto scoreObserver = go->AddComponent<dae::ScoreUIComponent>();

			pLevelFinishComp->AddScoreUIComponent(scoreObserver.get());
			playerScore->GetScoreGainedSubject()->AddObserver(scoreObserver.get());

		}

		//Player Health UI
		go = AddTextGameObject(scene, resourceManager, healthUIPos, "Score: ", "Lingua.otf", 16, UIColor); // 10, 250
		auto healthObserver = go->AddComponent<dae::LivesRemainingUIComponent>();

		playerHealth->GetPlayerDiedEventSubject()->AddObserver(healthObserver.get());
	}

#pragma endregion Player_Component_Setup


#pragma region Command_Setup
	auto sceneName = scene.GetName();
	if (canUseKeyboard)
	{
		auto pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_S, dae::ButtonState::Pressed, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::Down);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_S, dae::ButtonState::Up, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::None);


		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_W, dae::ButtonState::Pressed, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::Up);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_W, dae::ButtonState::Up, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::None);



		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_D, dae::ButtonState::Pressed, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::Right);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_D, dae::ButtonState::Up, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::None);


		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_A, dae::ButtonState::Pressed, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::Left);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_A, dae::ButtonState::Up, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::None);

	}

	auto pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadUp, dae::ButtonState::Pressed, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::Up);

	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadUp, dae::ButtonState::Up, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::None);


	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadDown, dae::ButtonState::Pressed, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::Down);

	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadDown, dae::ButtonState::Up, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::None);


	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadRight, dae::ButtonState::Pressed, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::Right);

	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadRight, dae::ButtonState::Up, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::None);


	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadLeft, dae::ButtonState::Pressed, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::Left);

	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadLeft, dae::ButtonState::Up, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::None);


#pragma endregion Command_Setup

#pragma region MakeChildPtr
	//Child
	auto childGO = std::make_shared <dae::GameObject>();
	childGO->Initialize();

	childGO->SetParent(pToReturn.get(), false);

#pragma endregion MakeChildPtr

#pragma region Child_Component_Setup
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


	switch (gameState)
	{
	case dae::GameState::SinglePlayer:
	{

		inputManager.AddKeyboardCommandToMap<dae::AttackCommand>(dae::KeyBoardInput{ SDL_SCANCODE_SPACE, dae::ButtonState::Down, sceneName }, std::make_unique<dae::AttackCommand>(attackComponent.get()));
		inputManager.AddControllerCommandToMap<dae::AttackCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::ButtonB, dae::ButtonState::Down, sceneName }
		, std::make_unique<dae::AttackCommand>(attackComponent.get()));

		//Player attack usage UI
		go = AddTextGameObject(scene, resourceManager, pepperUIPos, "Score: ", "Lingua.otf", 16, UIColor); // 10, 270		 
		auto pepperUseObserver = go->AddComponent<dae::PepperUsesUIComponent>();

		attackComponent->GetAttackSubject()->AddObserver(pepperUseObserver.get());
	}
	break;
	case dae::GameState::Coop:
	{

		if (!isPlayerTwo)
			inputManager.AddKeyboardCommandToMap<dae::AttackCommand>(dae::KeyBoardInput{ SDL_SCANCODE_SPACE, dae::ButtonState::Down, sceneName }, std::make_unique<dae::AttackCommand>(attackComponent.get()));

		inputManager.AddControllerCommandToMap<dae::AttackCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::ButtonB, dae::ButtonState::Down, sceneName }
		, std::make_unique<dae::AttackCommand>(attackComponent.get()));

		//Player attack usage UI
		go = AddTextGameObject(scene, resourceManager, pepperUIPos, "Score: ", "Lingua.otf", 16, UIColor); // 10, 270		 
		auto pepperUseObserver = go->AddComponent<dae::PepperUsesUIComponent>();

		attackComponent->GetAttackSubject()->AddObserver(pepperUseObserver.get());
	}
	break;
	case dae::GameState::Versus:
	{

		if (!isPlayerTwo)
		{
			inputManager.AddKeyboardCommandToMap<dae::AttackCommand>(dae::KeyBoardInput{ SDL_SCANCODE_SPACE, dae::ButtonState::Down, sceneName }, std::make_unique<dae::AttackCommand>(attackComponent.get()));
			inputManager.AddControllerCommandToMap<dae::AttackCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::ButtonB, dae::ButtonState::Down, sceneName }
			, std::make_unique<dae::AttackCommand>(attackComponent.get()));

			//Player attack usage UI
			go = AddTextGameObject(scene, resourceManager, pepperUIPos, "Score: ", "Lingua.otf", 16, UIColor); // 10, 270		 
			auto pepperUseObserver = go->AddComponent<dae::PepperUsesUIComponent>();

			attackComponent->GetAttackSubject()->AddObserver(pepperUseObserver.get());
		}
	}
	break;
	}


#pragma endregion Child_Component_Setup

	//Bind the method with what variable will call it and an X amount of parameters.
	auto childBindResult = std::bind(&dae::PlayerCollideCheckComponent::OnGameObjectCollision, playColComp, std::placeholders::_1, std::placeholders::_2);
	childColComp->SetOnTriggerEvent(childBindResult);


	childGO->GetTransform()->SetLocalPosition(halfTextureWidth / 2.f, 0, 0);

	return pToReturn;
}


std::shared_ptr<dae::GameObject> AddPlayerObjectWithScoreAndHealth(dae::Scene& scene, dae::ResourceManager& resourceManager, dae::InputManager& inputManager, unsigned int playerIndex,
	glm::vec2 position, glm::vec2 healthUIPos, glm::vec2 scoreUIPos, std::string imagePath,
	SDL_Color uiColor, bool canUseKeyboard)
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

	auto sceneName = scene.GetName();
	if (canUseKeyboard)
	{
		auto pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_S, dae::ButtonState::Pressed, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::Down);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_S, dae::ButtonState::Up, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::None);


		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_W, dae::ButtonState::Pressed, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::Up);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_W, dae::ButtonState::Up, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::None);



		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_D, dae::ButtonState::Pressed, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::Right);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_D, dae::ButtonState::Up, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::None);


		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_A, dae::ButtonState::Pressed, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::Left);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::ChangeDirectionCommand>(dae::KeyBoardInput{ SDL_SCANCODE_A, dae::ButtonState::Up, sceneName }, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
		pMoveCommand->SetDirection(dae::PlayerDirection::None);

	}

	auto pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadUp, dae::ButtonState::Pressed, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::Up);

	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadUp, dae::ButtonState::Up, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::None);


	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadDown, dae::ButtonState::Pressed, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::Down);

	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadDown, dae::ButtonState::Up, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::None);


	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadRight, dae::ButtonState::Pressed, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::Right);

	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadRight, dae::ButtonState::Up, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::None);


	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadLeft, dae::ButtonState::Pressed, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::Left);

	pMoveCommand = inputManager.AddControllerCommandToMap<dae::ChangeDirectionCommand>(dae::ControllerInput{ playerIndex, dae::Controller::ControllerButton::DPadLeft, dae::ButtonState::Up, sceneName }
	, std::make_unique<dae::ChangeDirectionCommand>(goRaw));
	pMoveCommand->SetDirection(dae::PlayerDirection::None);

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
