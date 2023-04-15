#pragma once
#include <string>
#include <memory>
#include <SDL_ttf.h>

#include "Component.h"
namespace dae
{
	class Font;
	class Texture2D;
	class GameObject;

	class TextObjectComponent final : public Component
	{
	public:
		TextObjectComponent(std::weak_ptr<GameObject> pOwner);
		~TextObjectComponent() = default;


		TextObjectComponent(const TextObjectComponent& other) = delete;
		TextObjectComponent(TextObjectComponent&& other) = delete;
		TextObjectComponent& operator=(const TextObjectComponent& other) = delete;
		TextObjectComponent& operator=(TextObjectComponent&& other) = delete;


		void Render() const override;
		void Update() override;

		void SetText(const std::string& text);
		void SetFont(std::shared_ptr<dae::Font> font);
		void SetColor(int r, int g, int b);
		std::shared_ptr<Texture2D> GetTexture() { return m_textTexture; }
	
	private:
		bool m_needsUpdate;
		std::string m_text;
		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture2D> m_textTexture;
		SDL_Color m_color;
	};
}
