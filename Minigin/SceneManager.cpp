#include "SceneManager.h"
#include "Scene.h"
#include <cassert>

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

void dae::SceneManager::SetActiveSceneByName(const std::string& name)								
{																									
	auto it = std::find_if(m_scenes.begin(), m_scenes.end(),										
		[&](const std::shared_ptr<dae::Scene>& scene)												
		{																							
			return std::strcmp(scene->GetName().c_str(), name.c_str()) == 0;						
		});																							
																									
	if (it != m_scenes.end())																		
		m_ActiveScene = *it;																			
}																									
																									
dae::Scene& dae::SceneManager::GetSceneByName(const std::string& name)								
{																									
	auto it = std::find_if(m_scenes.begin(), m_scenes.end(),										
		[&](const std::shared_ptr<dae::Scene>& scene)												
		{																							
			return std::strcmp(scene->GetName().c_str(), name.c_str()) == 0;						
		});																							
																									
	assert(it == m_scenes.end() && "Invalid argument: scene name doesn't exist!");					
																									
																									
	return **it;																					
																									
}

std::string dae::SceneManager::GetActiveSceneName() const
{
	return m_ActiveScene->GetName();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);

	//if (m_ActiveScene)
	//{
	//	m_ActiveScene->SetIsActive(false);
	//}
	//scene->SetIsActive(true);
	//m_ActiveScene = scene;
	return *scene;
}
