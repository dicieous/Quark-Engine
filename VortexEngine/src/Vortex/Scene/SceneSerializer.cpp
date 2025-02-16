#include "Vxpch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#define KEY(x) << YAML::Key << x
#define VALUE(x) << YAML::Value << x
#define KEYVAL(x,y) KEY(x)VALUE(y)

#define STARTMAP << YAML::BeginMap
#define ENDMAP << YAML::EndMap

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);

			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);

			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Vortex
{

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}


	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		:m_Scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::Newline;
		out STARTMAP; //Entity
		out KEYVAL("Entity", "12837192831273"); //TODO: Entity ID goes here

		if (entity.HasComponent<TagComponent>())
		{
			out KEY("TagComponent");
			out STARTMAP; // TagComponent

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out KEYVAL("Tag", tag);

			out ENDMAP; // TagComponent
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out KEY("TransformComponent");
			out STARTMAP; //TransformComponent

			auto& tranformComp = entity.GetComponent<TransformComponent>();

			out KEYVAL("Translation", tranformComp.Translation);
			out KEYVAL("Rotation", tranformComp.Rotation);
			out KEYVAL("Scale", tranformComp.Scale);

			out ENDMAP; //TransformComponent
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out KEY("CameraComponent");
			out STARTMAP; //CameraComponent

			auto& cameraComp = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComp.Camera;

			//SceneCamera
			camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective 
				? out KEYVAL("ProjectionType", "Perspective") 
				: out KEYVAL("ProjectionType", "Orthographic");
			
			out KEYVAL("OrthographicSize", camera.GetOrthographicSize());
			out KEYVAL("OrthographicNearClip", camera.GetOrthographicNearClip());
			out KEYVAL("OrthographicFarClip", camera.GetOrthographicFarClip());
			
			out KEYVAL("PerspectiveFOV", camera.GetPerspectiveVerticalFOV());
			out KEYVAL("PerspectiveFarClip", camera.GetPerspectiveFarClip());
			out KEYVAL("PerspectiveNearClip", camera.GetPerspectiveNearClip());

			//cameraComponent
			out KEYVAL("Primary Camera", cameraComp.primary);
			out KEYVAL("Fixed AspectRatio", cameraComp.FixedAspectRatio);

			out ENDMAP; //CameraComponent
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out KEY("SpriteRendererComponent");
			out STARTMAP;//SpriteRenderer

			auto& spriteComp = entity.GetComponent<SpriteRendererComponent>();
			out KEYVAL("Color", spriteComp.Color);
			
			out ENDMAP;//SpriteRenderer
		}

		out ENDMAP; //Entity
	}

	void SceneSerializer::Serialize(const std::string& filePath)
	{
		YAML::Emitter out;
		out STARTMAP;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.view<entt::entity>().each([&](auto entityID)
			{
				Entity entity = { entityID, m_Scene.get() };
				if (!entity)
					return;

				SerializeEntity(out, entity);
			});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filePath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filePath)
	{
		//Not implemented
		VX_CORE_ASSERT(false, "Not Implemented Yet");
	}

	bool SceneSerializer::Deserialize(const std::string& filePath)
	{
		std::ifstream stream(filePath);
		std::stringstream str;
		str << stream.rdbuf();

		YAML::Node data = YAML::Load(str.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		VX_CORE_TRACE("Deserialized Scene '{0}' ", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			std::vector<YAML::Node> entityList;
			for (auto entity : entities)
				entityList.push_back(entity);

			for (auto it = entityList.rbegin(); it != entityList.rend(); ++it)
			{
				auto& entity = *it;
				uint64_t uuid = entity["Entity"].as<uint64_t>();
				
				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();
					
				VX_CORE_TRACE("Deserialized Entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntity(name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					//Contains Transform by Default
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto spriteComponent = entity["SpriteRendererComponent"];
				if (spriteComponent)
				{
					auto& spriteC = deserializedEntity.AddComponent<SpriteRendererComponent>();
					spriteC.Color = spriteComponent["Color"].as<glm::vec4>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cameraC = deserializedEntity.AddComponent<CameraComponent>();
					cameraC.Camera.SetProjectionType(cameraComponent["ProjectionType"].as<std::string>() == "Perspective"
						? SceneCamera::ProjectionType::Perspective
						: SceneCamera::ProjectionType::Orthographic);
					
					cameraC.Camera.SetOrthographicSize(cameraComponent["OrthographicSize"].as<float>());
					cameraC.Camera.SetOrthoGraphicFarClip(cameraComponent["OrthographicFarClip"].as<float>());
					cameraC.Camera.SetOrthoGraphicNearClip(cameraComponent["OrthographicNearClip"].as<float>());
					
					cameraC.Camera.SetPerspectiveVerticalFOV(cameraComponent["PerspectiveFOV"].as<float>());
					cameraC.Camera.SetPerspectiveFarClip(cameraComponent["PerspectiveFarClip"].as<float>());
					cameraC.Camera.SetPerspectiveNearClip(cameraComponent["PerspectiveNearClip"].as<float>());

					
					cameraC.primary = cameraComponent["Primary Camera"].as<bool>();
					cameraC.FixedAspectRatio = cameraComponent["Fixed AspectRatio"].as<bool>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filePath)
	{
		//Not implemented
		VX_CORE_ASSERT(false, "Not Implemented Yet");
		return false;
	}
}