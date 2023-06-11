#pragma once
#include "Component.h"
#include "Renderer.h"

namespace dae
{
	class TransformComponent;
	class TextObjectComponent;

	class TextRenderComponent final : public Component
	{
	public:
		TextRenderComponent(GameObject* pOwner);

		~TextRenderComponent();
		TextRenderComponent(const TextRenderComponent& other) = delete;
		TextRenderComponent(TextRenderComponent&& other) = delete;
		TextRenderComponent& operator=(const TextRenderComponent& other) = delete;
		TextRenderComponent& operator=(TextRenderComponent&& other) = delete;

		void Render() const override;

		std::string m_nameTag{};

	private:
		TransformComponent* m_transformPosition;
		TextObjectComponent* m_textTexture;

	};


}

