#pragma once
#include "Component.h"

namespace dae
{
	class Font;
	class Texture2D;

	class ImageObjectComponent final : public Component
	{
	public:
		ImageObjectComponent(GameObject* pOwner);
		~ImageObjectComponent() = default;


		ImageObjectComponent(const ImageObjectComponent& other) = delete;
		ImageObjectComponent(ImageObjectComponent&& other) = delete;
		ImageObjectComponent& operator=(const ImageObjectComponent& other) = delete;
		ImageObjectComponent& operator=(ImageObjectComponent&& other) = delete;

		void SetTexture(std::shared_ptr<Texture2D> texture);

		std::shared_ptr<Texture2D> GetTexture() { return m_pTexture; }

	private:
		bool m_needsUpdate{false};
		std::shared_ptr<Texture2D> m_pTexture{nullptr};
	};
}
