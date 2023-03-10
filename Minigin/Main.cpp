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


	void load()
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
		auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
		auto& resourceManager = dae::ResourceManager::GetInstance();
		auto texture = resourceManager.LoadTexture("background.tga");
	
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
		auto parent = std::make_shared<dae::GameObject>();
		parent->Initialize();
		parent->AddComponent<dae::ImageObjectComponent>()->SetTexture(texture);
		parent->AddComponent<dae::ImageRenderComponent>();
		parent->AddComponent<dae::RotatorComponent>()->SetPivotPoint(100, 100, 0);
		parent->GetComponent<dae::TransformComponent>()->SetLocalPosition(100, 100,0);
		
		scene.Add(parent);


		texture = resourceManager.LoadTexture("Galaga_Butterfly.png");
		auto child = std::make_shared<dae::GameObject>();
		child->Initialize();
		child->SetParent(parent, false);
		child->AddComponent<dae::ImageObjectComponent>()->SetTexture(texture);
		child->AddComponent<dae::ImageRenderComponent>();
		child->AddComponent<dae::RotatorComponent>()->SetMovementSpeed(-10);

		scene.Add(child);
	}
	
	int main(int, char*[]) {
		dae::Minigin engine("../Data/");
		engine.Run(load);
	    return 0;
	}
