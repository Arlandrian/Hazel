#pragma once

#include "Hazel/Renderer/Shader.h"
#include <string>
#include <glm/glm.hpp>
#include <unordered_map>

// TODO: REMOVE!
typedef unsigned int GLenum;

namespace Hazel {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& srcPath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const std::string& GetName() const override { return m_Name; };
		virtual void SetName(const std::string& name) override { m_Name = name ; };

		virtual void SetInt(const std::string& name, const unsigned int value) const override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) const override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) const override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) const override;

		void UploadUniformInt(const std::string& name, int value) const ;

		void UploadUniformFloat (const std::string& name, float value) const ;
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values) const ;
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values) const ;
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values) const ;


		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const ;
	private:
		std::string ReadFile(const std::string& srcPath) const;
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}