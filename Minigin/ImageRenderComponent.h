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
	
		~ImageRenderComponent();
		ImageRenderComponent(const ImageRenderComponent& other) = delete;
		ImageRenderComponent(ImageRenderComponent&& other) = delete;
		ImageRenderComponent& operator=(const ImageRenderComponent& other) = delete;
		ImageRenderComponent& operator=(ImageRenderComponent&& other) = delete;
	
		void Render() const override;

		void SetSrcRect(SDL_Rect srcRect) { m_srcRect = srcRect; }
		SDL_Rect& GetSrcRect() { return m_srcRect; }

		void SetDestRect(SDL_Rect destRect) { m_destRect = destRect; }
		SDL_Rect& GetDestRect() { return m_destRect; }

		std::shared_ptr<Texture2D> GetTexture() const { return m_pTexture; }

	private:
		TransformComponent* m_pTransformPosition;
		ImageObjectComponent* m_pImageTexture;
		std::shared_ptr<Texture2D> m_pTexture{ nullptr };
		SDL_Rect m_srcRect{};
		SDL_Rect m_destRect{};

	};

}

