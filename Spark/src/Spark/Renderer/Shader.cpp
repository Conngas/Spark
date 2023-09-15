#pragma once
#include "spkpch.h"
#include "Spark/Renderer/Shader.h"
#include "Spark/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <string>
#include <glm/glm.hpp>

namespace Spark {

	//////////////////////////////////////////////////////////////////////////
	/// Shader
	//////////////////////////////////////////////////////////////////////////

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	SPK_CORE_ASSERT(false, "RendererAPI:: None is currently not supported!");
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLShader>(filepath);
		}
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	SPK_CORE_ASSERT(false, "RendererAPI:: None is currently not supported!");
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		SPK_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

	//////////////////////////////////////////////////////////////////////////
	/// ShaderLibrary
	//////////////////////////////////////////////////////////////////////////

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		// 找不到会返回末尾end()，因此 != 表示找到了，== 表示找不到
		SPK_CORE_ASSERT(!Exists(name), "Shader Already Exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	Spark::Ref<Spark::Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Spark::Ref<Spark::Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Spark::Ref<Spark::Shader> ShaderLibrary::Get(const std::string& name)
	{
		SPK_CORE_ASSERT(Exists(name), "Shader Not Found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}
