#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

namespace Vortex {

	class Renderer {

	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();


		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));


		inline static RendererAPI::API GetCurrentAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;

	};
}