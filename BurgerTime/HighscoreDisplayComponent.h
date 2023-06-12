#pragma once
#include "Component.h"


namespace dae {

	class TextObjectComponent;
	class TextRenderComponent;

	class HighscoreDisplayComponent final : public Component
	{
	public:
		HighscoreDisplayComponent(GameObject* pOwner);

		~HighscoreDisplayComponent() = default;
		HighscoreDisplayComponent(const HighscoreDisplayComponent& other) = delete;
		HighscoreDisplayComponent(HighscoreDisplayComponent&& other) = delete;
		HighscoreDisplayComponent& operator=(const HighscoreDisplayComponent& other) = delete;
		HighscoreDisplayComponent& operator=(HighscoreDisplayComponent&& other) = delete;

		void UpdateHighScore();

	private:
		std::vector<TextObjectComponent*> m_pTextObjectComp;
	};
}

