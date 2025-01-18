#include "SandBox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#if 0
static uint32_t s_MapWidth = 23;

static const char* s_MapTiles = 
"GGGGGGGGGGGGGGGGGGGGGGG"
"GFDDDDDDFGGGGGGGTGGGGGG"
"GFDDDDDDFGGGGGGGGGGGGGG"
"GFDDDDDDFGGGGGGGGGGGGGG"
"GGGGRGGGGGGWGGGGGGGGGGG"
"GGGGRGGGGGGRGGGGGGGGGGG"
"GGGGRGGGGGGRGGGGGTGGGGG"
"GGGGRGGGGGGRGGGGGGGGGGG"
"GGGGRRRRRRRRGGGGGGGGGGG"
"GGGGGGGTGGGGGHHHHGGGGGG"
"GGGGGGGGGGGGGHHHHGGGGGG"
"GGGGGGGGGGGGGGGGGGGGGGG"
;

static uint32_t s_MapHeight = strlen(s_MapTiles) / s_MapWidth;
#endif

SandBox2D::SandBox2D() :
	Layer("SandBox2D"), m_CameraController(1280.0f / 720.0f, true) {}

void SandBox2D::OnAttach()
{
	m_checkerBoardTexture = Vortex::Texture2D::Create("Assets/Textures/Checkerboard.png");
	
#if 0
	m_SpriteSheet = Vortex::Texture2D::Create("Assets/Textures/tilemap_packed.png");

	m_TextureTree = Vortex::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 4, 8 }, { 16, 16 });
	m_TextureMap['G'] = Vortex::SubTexture2D::CreateFromCoords(m_SpriteSheet, {1, 10}, {16, 16});
	m_TextureMap['F'] = Vortex::SubTexture2D::CreateFromCoords(m_SpriteSheet, {11, 6}, {16, 16});
	m_TextureMap['D'] = Vortex::SubTexture2D::CreateFromCoords(m_SpriteSheet, {0, 10}, {16, 16});
	m_TextureMap['R'] = Vortex::SubTexture2D::CreateFromCoords(m_SpriteSheet, {7, 7},  {16, 16});
	m_TextureMap['T'] = Vortex::SubTexture2D::CreateFromCoords(m_SpriteSheet, {3, 8},  {16, 16});
	m_TextureMap['W'] = Vortex::SubTexture2D::CreateFromCoords(m_SpriteSheet, {7, 2},  {16, 16});
	m_TextureMap['H'] = Vortex::SubTexture2D::CreateFromCoords(m_SpriteSheet, {2, 10}, {16, 16});

	m_CameraController.SetZoomLevel(5.0f);

	m_ParticleProp.ColorBegin = { 61 / 255.0f, 255 / 255.0f, 246 / 255.0f, 1.0f };
	m_ParticleProp.ColorEnd = { 155 / 255.0f, 77 / 255.0f, 255 / 255.0f, 1.0f };
	m_ParticleProp.SizeBegin = 0.25f, m_ParticleProp.SizeVariation = 0.3f, m_ParticleProp.SizeEnd = 0.0f;
	m_ParticleProp.LifeTime = 5.0f;
	m_ParticleProp.Velocity = { 0.0f, 0.0f };
	m_ParticleProp.VelocityVariation = { 3.0f, 1.0f };
	m_ParticleProp.Position = { 0.0f, 0.0f };
#endif
}

void SandBox2D::OnDetach()
{

}

void SandBox2D::OnUpdate(Vortex::TimeStep timeStep)
{
	VX_PROFILE_FUNCTION();
	//OnUpdate

	{
		VX_PROFILE_SCOPE("CameraController");
		m_CameraController.OnUpdate(timeStep);
	}

	//Rendering
	Vortex::Renderer2D::ResetStats();
	{
		VX_PROFILE_SCOPE("RenderPrep");
		Vortex::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Vortex::RenderCommand::Clear();
	}

	{
		VX_PROFILE_SCOPE("SandBox2D::BeginScene");
		Vortex::Renderer2D::BeginScene(m_CameraController.GetCamera());
	}

	{
		static float rotation = 0.0f;
		rotation += 2.0f;

		VX_PROFILE_SCOPE("RenderDraw");

		Vortex::Renderer2D::DrawRotatedQuads({ -1.0f,1.0f }, { 0.8f,0.8f }, glm::radians(45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
		Vortex::Renderer2D::DrawQuads({ -1.0f,0.0f }, { 0.8f,0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Vortex::Renderer2D::DrawQuads({ 0.5f,-0.5f }, { 0.5f,0.75f }, { 0.2f, 0.2f, 0.3f, 1.0f });
		Vortex::Renderer2D::DrawQuads({ 0.0f, 0.0f, -0.1f }, { 20.0f,20.0f }, m_checkerBoardTexture, 10.0f);
		Vortex::Renderer2D::DrawRotatedQuads({ -2.0f, 0.0f, 0.0f }, { 1.0f,1.0f }, glm::radians(rotation), m_checkerBoardTexture, 20.0f, { 1.0f, 0.9f, 0.9f, 1.0f });

		for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {

				glm::vec4 color = { ((x + 5.0f) / 10.0f), 0.3f, ((y + 5.0f) / 10.0f),0.7f };
				Vortex::Renderer2D::DrawQuads({ x, y }, { 0.45f,0.45f }, color);
			}
		}
	}

	Vortex::Renderer2D::EndScene();

#if 0
	if (Vortex::Input::IsMouseButtonPressed(VX_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Vortex::Input::GetMousePosition();
		auto width = Vortex::Application::Get().GetWindow().GetWidth();
		auto height = Vortex::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_ParticleProp.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 10; i++)
			m_ParticleSystem.Emit(m_ParticleProp);
	}

	m_ParticleSystem.OnUpdate(timeStep);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());

	Vortex::Renderer2D::BeginScene(m_CameraController.GetCamera());

	for (uint32_t y = 0; y < s_MapHeight; y++)
	{
		for (uint32_t x = 0; x < s_MapWidth; x++)
		{
			char spriteText = s_MapTiles[x + y * s_MapWidth];
			if (m_TextureMap.find(spriteText) != m_TextureMap.end()) 
			{
				Vortex::Renderer2D::DrawQuads({ x - s_MapWidth / 2.0f, s_MapHeight - y - s_MapHeight / 2.0f, 0.2f }, { 1.0f, 1.0f }, m_TextureMap[spriteText]);
			}
		}
	}

	//Vortex::Renderer2D::DrawQuads({ 0.0f, 0.0f, 0.3f }, { 1.0f, 2.0f }, m_TextureTree);
#endif

	Vortex::Renderer2D::EndScene();

}

void SandBox2D::OnImGuiRender()
{
	VX_PROFILE_FUNCTION();

	auto stats = Vortex::Renderer2D::GetStats();


	static bool dockSpaceOpen = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Docking"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			if (ImGui::MenuItem("Exit")) Vortex::Application::Get().Close();

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::Begin("Batching Test Rendering");

	ImGui::Text("2D Batch Renderer Stats");

	ImGui::Text("Vertices : %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices : %d", stats.GetTotalIndexCount());
	ImGui::Text("Draw Calls : %d", stats.DrawCalls);
	ImGui::Text("Quads : %d", stats.QuadCount);

	ImGui::Image(m_checkerBoardTexture->GetRendererID(), ImVec2{ 256.0f, 256.0f });

	ImGui::End();

	ImGui::End();
}

void SandBox2D::OnEvent(Vortex::Event& event)
{
	m_CameraController.OnEvent(event);
}
