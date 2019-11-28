#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Hazel {
	class Shader {
	public:
		
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;
		virtual void SetName(const std::string& name) = 0;

		static Ref<Shader> Create(const std::string& srcPath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		virtual void SetInt(const std::string& name, const unsigned int value) const = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) const = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) const = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) const = 0;

	};

	class ShaderLibrary {
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filePath); //assets/Texture.glsl
		Ref<Shader> Load(const std::string& name, const std::string& filePath);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name);
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}
