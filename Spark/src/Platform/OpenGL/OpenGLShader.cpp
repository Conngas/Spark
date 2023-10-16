#include "spkpch.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Spark {

	static GLenum ShaderTypeFromString(std::string& type)
	{
		SPK_PROFILE_FUNCTION();

		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		SPK_CORE_ASSERT(false, "Unkown Shader Type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		SPK_PROFILE_FUNCTION();

		// ��ȡ������Shader
		std::string source = ReadFile(filepath);
		auto shaderSource = PreProcess(source);
		Compile(shaderSource);

		// ��ȡ����
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - 1 : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		SPK_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> source;
		source[GL_VERTEX_SHADER] = vertexSrc;
		source[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(source);
	}

	OpenGLShader::~OpenGLShader()
	{
		SPK_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		SPK_PROFILE_FUNCTION();

		std::string readInRes;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);	// ifstream close itself due to RAII ���Զ��ر�
		if (in)
		{
			// ��ȡ���ļ�ĩβ
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				readInRes.resize(in.tellg());						// Resize �����ַ��������С��reserve Ԥ����С��Ҫָ��
				in.seekg(0, std::ios::beg);
				in.read(&readInRes[0], readInRes.size());
				in.close();
			}
			else
				SPK_CORE_ASSERT("File '{0}' is Empty", filepath);
		}
		else
		{
			SPK_CORE_ASSERT("Could Not Open File '{0}'", filepath);
		}
		return readInRes;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		SPK_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSource;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);				// �ҵ���һ�о��ӵĽ�β�����ڷ���#���ͣ�\r �س� \n ���У�
			SPK_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);		// ��ȡtype���ݣ�substr����ʼ�㣬���ȣ�
			SPK_CORE_ASSERT(ShaderTypeFromString(type), "Invail Shader type specified!");
			size_t nextlinePos = source.find_first_not_of("\r\n", eol);	// �ҵ��µ�һ�У�find_first_not_of���������ݣ���ʼ�㣩
			pos = source.find(typeToken, nextlinePos);					// �ҵ���һ��Token��λ�ã����µ�һ�п�ʼ��find���������ݣ���ʼ�㣩��δ�ҵ������ļ�ĩβ
			shaderSource[ShaderTypeFromString(type)] =
				source.substr(nextlinePos, pos - (nextlinePos == std::string::npos ? source.size() - 1 : nextlinePos));
																		// ĩβ�ҵ�token��pos - newline �õ��м�һ������
																		// ĩβδ�ҵ�token��pos - newline �õ��м�����
		}
		return shaderSource;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSource)
	{
		SPK_PROFILE_FUNCTION();

		SPK_CORE_ASSERT(shaderSource.size() >0 && shaderSource.size() <=2, "Wrong Shader Number, Please Check Shader!");
		GLuint program = glCreateProgram();
		std::array<GLuint, 2> glShaderIDs;
		int glShaderIndex = 0;
		for (auto& kv : shaderSource)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1,&sourceCStr, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				// ����ʧ��ʱִ��
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// �ͷ���ɫ��
				glDeleteShader(shader);

				SPK_CORE_ERROR("{0}", infoLog.data()); // �ȼ���&infoLog[0]
				SPK_CORE_ASSERT(false, "Vertex Shader Compilation failure!");
				break;
			}
			glAttachShader(program, shader);
			glShaderIDs[glShaderIndex++] = shader;
		}
		m_RendererID = program;
		// ���ӳ���
		glLinkProgram(program);

		// ����ʧ��ִ��
		GLint isLink = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLink);
		if (isLink == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			glDeleteShader(program);
			for (auto id : glShaderIDs)
				glDeleteShader(id);			
			SPK_CORE_ERROR("{0}", infoLog.data());
			SPK_CORE_ASSERT(false, "Shader Linke Failure!");
			return;
		}

		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
			
	}

	void OpenGLShader::Bind() const
	{
		SPK_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnBind() const
	{
		SPK_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		SPK_PROFILE_FUNCTION();

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* value, uint32_t count)
	{
		SPK_PROFILE_FUNCTION();

		UploadUniformIntArray(name, value, count);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		SPK_PROFILE_FUNCTION();

		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		SPK_PROFILE_FUNCTION();

		UploadUniformFloat3(name, value);
	}
		
	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		SPK_PROFILE_FUNCTION();

		UploadUniformFloat4(name, value);
	}
		
	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		SPK_PROFILE_FUNCTION();

		UploadUniformMat4(name, value);
	}
	//////////////////////////////////////////////////////////////////////////
	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* value, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, values);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, values.x,values.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		// ����󶨲���
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}