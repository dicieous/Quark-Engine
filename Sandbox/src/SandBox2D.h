#pragma once
#include <Vortex.h>

#include "ParticleSystem.h"

class SandBox2D : public Vortex::Layer {

public:
	SandBox2D();
	virtual ~SandBox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Vortex::TimeStep timeStep) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Vortex::Event& event) override;

private:
	Vortex::OrthographicCameraController m_CameraController;

	//Temp
	Vortex::Ref<Vortex::VertexArray> m_squareVA;

	Vortex::Ref<Vortex::Texture2D> m_checkerBoardTexture;
	Vortex::Ref<Vortex::Texture2D> m_SpriteSheet;
	Vortex::Ref<Vortex::SubTexture2D> m_TextureTree;

	Vortex::Ref<Vortex::Shader> m_flatColorShaderSqr;

	glm::vec3 m_squareColor{ 0.2f, 0.3f, 0.8f };

	ParticleProps m_ParticleProp;
	ParticleSystem m_ParticleSystem;
};