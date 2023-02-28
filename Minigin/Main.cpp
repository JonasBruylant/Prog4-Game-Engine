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
#include "TextObject.h"
#include "Transform.h"
#include "RenderComponent.h"
#include "FPSCountComponent.h"
#include "Scene.h"


	void load()
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
		auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
		auto go = std::make_shared<dae::GameObject>();
		go->AddComponent<dae::TextObjectComponent>()->SetFont(font);
		go->GetComponent<dae::TextObjectComponent>()->SetText("0");
		go->AddComponent<dae::TransformComponent>()->SetPosition(10, 10, 0);
		go->AddComponent<dae::RenderComponent>();
		go->AddComponent<dae::FPSCountComponent>();

		scene.Add(go);
	
		//go = std::make_shared<dae::GameObject>();
		//
		//scene.Add(go);
	
		//auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
		//auto to = std::make_shared<dae::TextObject>("Programming 4 Assignment", font);
		//to->SetPosition(80, 20);
		//scene.Add(to);
	}
	
	int main(int, char*[]) {
		dae::Minigin engine("../Data/");
		engine.Run(load);
	    return 0;
	}
