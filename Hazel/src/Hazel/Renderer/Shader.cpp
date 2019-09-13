#include "hzpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel {

	Ref<Shader> Shader::Create(const std::string& srcPath) {
		switch( Renderer::GetAPI() ) {
			case RendererAPI::API::None:	HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLShader>(srcPath);

		}
		HZ_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch( Renderer::GetAPI() ) {
			case RendererAPI::API::None:	HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);

		}
		HZ_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}


	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)	{
		HZ_CORE_ASSERT(!Exists(name) , "Shader already exists!");
		m_Shaders[name] = shader;

	}

	void ShaderLibrary::Add(const Ref<Shader>& shader) {
		auto& name = shader->GetName();
		Add(name, shader);
	}


	Ref<Shader> ShaderLibrary::Load(const std::string& filePath) {
		auto shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath) {
		auto shader = Shader::Create(filePath);//PROBLEM// this will create a shader with file name but add it to the library with given name
		shader->SetName(name);
		Add(name, shader); 
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name) {
		HZ_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name)	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

}