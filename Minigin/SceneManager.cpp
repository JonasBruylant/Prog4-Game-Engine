#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	//for(auto& scene : m_scenes)
	//{
	//	scene->Update();
	//}
	m_ActiveScene->Update();
}

void dae::SceneManager::Render()
{
	//for (const auto& scene : m_scenes)
	//{
	//	scene->Render();
	//}

	m_ActiveScene->Render();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);

	if (m_ActiveScene)
	{
		m_ActiveScene->SetIsActive(false);
	}
	scene->SetIsActive(true);
	m_ActiveScene = scene;
	return *scene;
}
