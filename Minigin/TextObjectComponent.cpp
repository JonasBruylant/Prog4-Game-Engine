#include "TextObjectComponent.h"
#include <stdexcept>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "Gameobject.h"

dae::TextObjectComponent::TextObjectComponent(GameObject* pOwner):
	Component(pOwner),
	m_needsUpdate{false}
{
	m_color = { static_cast<Uint8>(255), static_cast<Uint8>(255), static_cast<Uint8>(255) };
}


void dae::TextObjectComponent::Update()
{
	if (m_needsUpdate)
	{
		
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_textTexture = std::make_shared<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

void dae::TextObjectComponent::Render() const
{

}

// This implementation uses the "dirty flag" pattern
void dae::TextObjectComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}


void dae::TextObjectComponent::SetFont(std::shared_ptr<dae::Font> font)
{
	m_font = font;
	m_needsUpdate = true;
}

void dae::TextObjectComponent::SetColor(int r, int g, int b)
{
	m_color = {static_cast<Uint8>(r),static_cast<Uint8>(g),static_cast<Uint8>(b)};
	m_needsUpdate = true;
}


