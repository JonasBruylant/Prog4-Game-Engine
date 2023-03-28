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
#include "TextObjectComponent.h"
#include "Transform.h"
#include "TextRenderComponent.h"
#include "FPSCountComponent.h"
#include "Scene.h"
#include "ImageObjectComponent.h"
#include "ImageRenderComponent.h"
#include "RotatorComponent.h"
#include "TrashTheCacheComponent.h"
#include "InputManager.h"

#include "ControllerInput.h"
#include "Command.h"
#include "MoveCommand.h"

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
		go->Initialize();
		go->AddComponent<dae::TextObjectComponent>()->SetFont(font);
		go->GetComponent<dae::TextObjectComponent>()->SetText("0");
		go->AddComponent<dae::TextRenderComponent>();
		go->AddComponent<dae::FPSCountComponent>();

		go->GetComponent<dae::TransformComponent>()->SetLocalPosition(10,10,0);

		scene.Add(go);


		texture = resourceManager.LoadTexture("Galaga_Bee.png");
		go = std::make_shared<dae::GameObject>();
		go->Initialize();
		go->AddComponent<dae::ImageObjectComponent>()->SetTexture(texture);
		go->AddComponent<dae::ImageRenderComponent>();
		go->GetComponent<dae::TransformComponent>()->SetLocalPosition(100, 100,0);
		

		int movementSpeed = 100;
		auto pMoveCommand = inputManager.AddControllerCommandToMap<dae::MoveCommand>(0, dae::ControllerInput::ControllerButton::DPadDown, go);
		pMoveCommand->SetDirection(dae::MoveCommand::Direction::Down);
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::MoveCommand>(0, dae::ControllerInput::ControllerButton::DPadUp, go);
		pMoveCommand->SetDirection(dae::MoveCommand::Direction::Up);
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::MoveCommand>(0, dae::ControllerInput::ControllerButton::DPadRight, go);
		pMoveCommand->SetDirection(dae::MoveCommand::Direction::Right);
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddControllerCommandToMap<dae::MoveCommand>(0, dae::ControllerInput::ControllerButton::DPadLeft, go);
		pMoveCommand->SetDirection(dae::MoveCommand::Direction::Left);
		pMoveCommand->SetMovementSpeed(movementSpeed);

		scene.Add(go);


		texture = resourceManager.LoadTexture("Galaga_Butterfly.png");
		go = std::make_shared<dae::GameObject>();
		go->Initialize();
		go->AddComponent<dae::ImageObjectComponent>()->SetTexture(texture);
		go->AddComponent<dae::ImageRenderComponent>();
		go->GetComponent<dae::TransformComponent>()->SetLocalPosition(150, 150, 0);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::MoveCommand>(SDL_SCANCODE_S, go);
		pMoveCommand->SetDirection(dae::MoveCommand::Direction::Down);
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::MoveCommand>(SDL_SCANCODE_W, go);
		pMoveCommand->SetDirection(dae::MoveCommand::Direction::Up);
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::MoveCommand>(SDL_SCANCODE_D, go);
		pMoveCommand->SetDirection(dae::MoveCommand::Direction::Right);
		pMoveCommand->SetMovementSpeed(movementSpeed);

		pMoveCommand = inputManager.AddKeyboardCommandToMap<dae::MoveCommand>(SDL_SCANCODE_A, go);
		pMoveCommand->SetDirection(dae::MoveCommand::Direction::Left);
		pMoveCommand->SetMovementSpeed(movementSpeed);


		scene.Add(go);
	}
	
	int main(int, char*[]) {
		dae::Minigin engine("../Data/");
		engine.Run(load);
	    return 0;
	}
