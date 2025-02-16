#pragma once

#include <glm/glm.hpp>
#include "Texture.h"

namespace Vortex {

	class SubTexture2D {

	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, glm::vec2& max);

		const Ref<Texture2D> GetTexture() { return m_Texture; }
		const glm::vec2* GetTexCoords() { return m_TexCoords; }

		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords,const glm::vec2& cellSize, const glm::vec2& spriteRatio = {1, 1});

	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];
	};
}