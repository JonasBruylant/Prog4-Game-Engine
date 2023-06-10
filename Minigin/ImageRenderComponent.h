#pragma once
#include "Component.h"
#include "Renderer.h"

namespace dae {

	class TransformComponent;
	class ImageObjectComponent;
	class Texture2D;

	class ImageRenderComponent final : public Component
	{
	public:
		ImageRenderComponent(GameObject* pOwner);
	
		~ImageRenderComponent() = default;
		ImageRenderComponent(const ImageRenderComponent& other) = delete;
		ImageRenderComponent(ImageRenderComponent&& other) = delete;
		ImageRenderComponent& operator=(const ImageRenderComponent& other) = delete;
		ImageRenderComponent& operator=(ImageRenderComponent&& other) = delete;
	
		void Render() const override;
		void Update() override;

		void SetSrcRect(SDL_Rect srcRect) { m_srcRect = srcRect; }
		SDL_Rect& GetSrcRect() { return m_srcRect; }

		void SetDestRect(SDL_Rect destRect) { m_destRect = destRect; }
		SDL_Rect& GetDestRect() { return m_destRect; }

		void SetRenderUsingRects(bool render) { m_renderUsingRects = render; }
		bool GetRenderUsingRects() { return m_renderUsingRects; }


	private:
		TransformComponent* m_pTransformPosition;
		ImageObjectComponent* m_pImageTexture;
		SDL_Rect m_srcRect{};
		SDL_Rect m_destRect{};
		bool m_renderUsingRects{ false };
	};

}

