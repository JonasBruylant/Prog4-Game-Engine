#include "ImageObjectComponent.h"

dae::ImageObjectComponent::ImageObjectComponent(std::weak_ptr<GameObject> pOwner) : Component(pOwner)
{
}

void dae::ImageObjectComponent::SetTexture(std::shared_ptr<Texture2D> texture)
{
	m_pTexture = texture;
}
