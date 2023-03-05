#pragma once
#include "Component.h"
#include "Renderer.h"

namespace dae {

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
};

}
