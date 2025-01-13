#pragma once
#include "Camera.h"
#include "Texture.h"

namespace Vortex {

	class Renderer2D {

	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		//primitives////
		static void DrawQuads(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color);
		static void DrawQuads(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color);

		static void DrawQuads(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuads(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);
	};
}