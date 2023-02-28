#pragma once
#include <string>
#include <memory>
#include "Component.h"
#include "GameObject.h"

namespace dae
{
	class Font;
	class Texture2D;
	//class Component;

	class TextObjectComponent final : public Component
	{
	public:
		TextObjectComponent();
		TextObjectComponent(const std::string& text, std::shared_ptr<Font> font);
		virtual ~TextObjectComponent() = default;


		TextObjectComponent(const TextObjectComponent& other) = delete;
		TextObjectComponent(TextObjectComponent&& other) = delete;
		TextObjectComponent& operator=(const TextObjectComponent& other) = delete;
		TextObjectComponent& operator=(TextObjectComponent&& other) = delete;


		void Render() const override;
		void Update(float deltaTime) override;

		void SetText(const std::string& text);
		void SetFont(std::shared_ptr<dae::Font> font);
		//void SetPosition(float x, float y);
	private:
		bool m_needsUpdate;
		std::string m_text;
		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture2D> m_textTexture;
	};
}
