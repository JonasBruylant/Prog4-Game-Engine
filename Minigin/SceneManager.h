#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update();
		void Render();

		void Quit();
	
		void SetActiveSceneByName(const std::string& name);
		Scene& GetSceneByName(const std::string& name);
		std::string GetActiveSceneName() const;
		void RemoveSceneByName(const std::string& name);

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;
		std::shared_ptr<Scene> m_ActiveScene;
	};
}
