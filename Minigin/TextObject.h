#pragma once
#include <string>
#include <memory>
#include "GameObject.h"
#include "Component.h"

namespace dae
{
	class Font;
	class Texture2D;
	class Component;

	class TextObjectComponent final : public Component
	{
	public:

		TextObjectComponent(const std::string& text, std::shared_ptr<Font> font);
		virtual ~TextObjectComponent() = default;


		TextObjectComponent(const TextObjectComponent& other) = delete;
		TextObjectComponent(TextObjectComponent&& other) = delete;
		TextObjectComponent& operator=(const TextObjectComponent& other) = delete;
		TextObjectComponent& operator=(TextObjectComponent&& other) = delete;


		virtual void Update(float deltaTime);
		virtual void Render() const;

		void SetText(const std::string& text);
		//void SetPosition(float x, float y);
	private:
		bool m_needsUpdate;
		std::string m_text;
		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture2D> m_textTexture;
	};
}
