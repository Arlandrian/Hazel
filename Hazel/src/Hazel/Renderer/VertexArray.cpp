#include "hzpch.h"
#include "VertexArray.h"
#include "Hazel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel{
	void VertexArray::Bind() const { }

	void VertexArray::Unbind() const { }
	
	void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) { }
	
	void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) { }
	
	VertexArray* VertexArray::Create() {
		switch( Renderer::GetAPI() ) {
			case RendererAPI::API::None:		HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return new OpenGLVertexArray();
		}
		HZ_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}
}