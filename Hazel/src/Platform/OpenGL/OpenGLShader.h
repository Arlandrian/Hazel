#pragma once

#include "Hazel/Renderer/Shader.h"

namespace Hazel {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadUniformInt(const std::string& name, int value) const ;

		void UploadUniformFloat (const std::string& name, float value) const ;
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values) const ;
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values) const ;
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values) const override;


		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const override;


	private:
		uint32_t m_RendererID;
	};
}