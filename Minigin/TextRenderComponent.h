#pragma once
#include "Component.h"
#include "Renderer.h"

namespace dae
{

	class TextRenderComponent final : public Component
	{
	public:
		TextRenderComponent(std::weak_ptr<GameObject> pOwner);

		~TextRenderComponent();
		TextRenderComponent(const TextRenderComponent& other) = delete;
		TextRenderComponent(TextRenderComponent&& other) = delete;
		TextRenderComponent& operator=(const TextRenderComponent& other) = delete;
		TextRenderComponent& operator=(TextRenderComponent&& other) = delete;

		void Render() const override;


	private:
	};


}

