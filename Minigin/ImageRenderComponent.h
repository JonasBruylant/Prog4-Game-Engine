#pragma once
#include "Component.h"
#include "Renderer.h"

namespace dae {

	class TransformComponent;
	class ImageObjectComponent;


	class ImageRenderComponent : public Component
	{
	public:
		ImageRenderComponent(std::weak_ptr<GameObject> pOwner);
	
		~ImageRenderComponent();
		ImageRenderComponent(const ImageRenderComponent& other) = delete;
		ImageRenderComponent(ImageRenderComponent&& other) = delete;
		ImageRenderComponent& operator=(const ImageRenderComponent& other) = delete;
		ImageRenderComponent& operator=(ImageRenderComponent&& other) = delete;
	
		void Render() const override;
	
	private:
		std::weak_ptr<TransformComponent> m_pTransformPosition;
		std::weak_ptr<ImageObjectComponent> m_pImageTexture;
	};

}

