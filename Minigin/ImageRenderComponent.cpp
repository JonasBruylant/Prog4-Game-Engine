#include "ImageRenderComponent.h"

dae::ImageRenderComponent::ImageRenderComponent(std::weak_ptr<GameObject> pOwner):
	Component(pOwner)
{
}

dae::ImageRenderComponent::~ImageRenderComponent()
{
}

void dae::ImageRenderComponent::Render() const
{
	//auto texture = GetOwner().lock()->GetComponent<Texture2D>();
}
