#include "hzpch.h"
#include "VertexArray.h"
#include "Hazel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel{
	void VertexArray::Bind() const { }

	void VertexArray::Unbind() const { }
	
	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) { }
	
	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) { }
	
	VertexArray* VertexArray::Create() {
		switch( Renderer::GetAPI() ) {
			case RendererAPI::None:		HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:	return new OpenGLVertexArray();
		}
		HZ_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}
}