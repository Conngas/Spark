#include "Sandbox2D.h"
#include <imgui/imgui.h>
#include <Spark.h>
#include <Spark/Core/EntryPoint.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


class ExampleLayer : public Spark::Layer
{
public:
	ExampleLayer() :Layer("Example"), m_CameraController(1280.0f / 720.0f,true)
	{
		m_VertexArray = (Spark::VertexArray::Create());

		float vertices[3 * 7] =
		{
			-0.5f,	-0.5f,	0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f,	-0.5f,	0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.0f,	0.5f,	0.0f, 1.0f, 0.0f, 1.0f, 1.0f
		};

		m_VertexBuffer = Spark::VertexBuffer::Create(vertices, sizeof(vertices));
		// ��װlayout
		{
			Spark::BufferLayout layout =
			{
				{ Spark::ShaderDataType::Float3, "a_Position"},
				{ Spark::ShaderDataType::Float4, "a_Color"}
			};

			m_VertexBuffer->SetLayout(layout);
			m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		}


		uint32_t indices[3] = { 0,1,2 };
		m_IndexBuffer = Spark::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		// Shader���ԣ�����R"( ����ʵ�ֿ����ַ�����v_�仯�ַ������ڶ����ɫ����ͨ����
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;
			
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position,1.0f);
			}			
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position*0.5 + 0.5,1.0);
				color = v_Color;
			}
		)";

		m_Shader = Spark::Shader::Create("VertexPosShader", vertexSrc, fragmentSrc);

		//////////////////////////////////////////////////////////////////////////
		// Spark�л���һ�������εĲ���
		//////////////////////////////////////////////////////////////////////////
		// 1. h����ӱ���
		// 2. ����VertexArray
		m_SquareVartexArray = Spark::VertexArray::Create();
		// 3. ��������
		float squareVertices[5 * 4] =
		{
			-0.05f,	-0.05f,	0.0f, 0.0f, 0.0f,
			0.05f,	-0.05f,	0.0f, 1.0f, 0.0f,
			0.05f,	0.05f,	0.0f, 1.0f, 1.0f,
			-0.05f,	0.05f,	0.0f, 0.0f, 1.0f
		};
		// 4. SVB�趨
		m_SquareVertexBuffer = Spark::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		m_SquareVertexBuffer->SetLayout({ 
				{Spark::ShaderDataType::Float3,"a_Position"},
				{Spark::ShaderDataType::Float2,"a_TexCoord"}
			});
		m_SquareVartexArray->AddVertexBuffer(m_SquareVertexBuffer);
		// 5. �趨˳��
		uint32_t squareIndice[6] = { 0,1,2,2,3,0 };
		m_SquareIndexBuffer = Spark::IndexBuffer::Create(squareIndice, sizeof(squareIndice) / sizeof(uint32_t));
		m_SquareVartexArray->SetIndexBuffer(m_SquareIndexBuffer);
		// 6. дShader
		std::string SquareShaderVertexSrc = R"(
			#version 330 core
			layout(location = 0)in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position,1.0);
			}
		)";

		std::string SquareShaderFragmentSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color,1.0f);
			}
		)";
		m_SquareShader = Spark::Shader::Create("FlatColor", SquareShaderVertexSrc, SquareShaderFragmentSrc);
		// SquareTextureShader ����
		auto textureShader = shaderLibrary.Load("Assets/Shader/Texture.glsl");
		m_Texture = Spark::Texture2D::Create("Assets/Texture/Checkerboard.png");
		m_LogoTexture = Spark::Texture2D::Create("Assets/Texture/ChernoLogo.png");

		textureShader->Bind();
		textureShader->SetInt("u_Texture", 0);
	}

	void OnUpdate(Spark::Timestep ts) override
	{
		//////////////////////////////////////////////////////////////////////////
		// Update ����
		//////////////////////////////////////////////////////////////////////////
		m_CameraController.OnUpdate(ts);

		//////////////////////////////////////////////////////////////////////////
		// Renderer ���󲿷֣�
		//////////////////////////////////////////////////////////////////////////
		Spark::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Spark::RenderCommand::Clear();
	
		Spark::Renderer::BeginScene(m_CameraController.GetCamera());

		// Material Test
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)) * scale;

		m_SquareShader->Bind();
		m_SquareShader->SetFloat3("u_Color", m_CustomColor);

		for (int j = 0; j < 20; ++j)
		{
			for (int i = 0; i < 20; ++i)
			{
				glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos);
				Spark::Renderer::Submit(m_SquareShader, m_SquareVartexArray, transform);
			}
		}		

		auto textureShader = shaderLibrary.Get("Texture");

		m_Texture->Bind();
		// Submit Square
		Spark::Renderer::Submit(textureShader, m_SquareVartexArray, glm::scale(glm::mat4(1.0f), glm::vec3(15.0f)));
		
		m_LogoTexture->Bind();
		Spark::Renderer::Submit(textureShader, m_SquareVartexArray, glm::scale(glm::mat4(1.0f), glm::vec3(15.0f)));


		
		// Triangle Shader
		// Spark::Renderer::Submit(m_Shader, m_VertexArray);

		Spark::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_CustomColor));
		ImGui::End();
	}

	void OnEvent(Spark::Event& event) override
	{
		// ����Camera�¼�
		m_CameraController.OnEvent(event);
	}

private:
	Spark::ShaderLibrary shaderLibrary;

	Spark::Ref<Spark::Shader> m_Shader;
	Spark::Ref<Spark::VertexBuffer> m_VertexBuffer;
	Spark::Ref<Spark::VertexArray> m_VertexArray;
	Spark::Ref<Spark::IndexBuffer> m_IndexBuffer;

	Spark::Ref<Spark::Shader> m_SquareShader;
	Spark::Ref<Spark::VertexArray> m_SquareVartexArray;
	Spark::Ref<Spark::VertexBuffer> m_SquareVertexBuffer;
	Spark::Ref<Spark::IndexBuffer> m_SquareIndexBuffer;

	Spark::Ref<Spark::Texture2D> m_Texture,m_LogoTexture;

	Spark::OrthographicCameraController m_CameraController;
	glm::vec3 m_CustomColor = {0.2f, 0.2f, 0.8f};
}; 


/// <summary>
/// �̳�Spark�����е�Applicationʵ��
/// </summary>
class Sandbox : public Spark::Application
{
public:
	Sandbox()
	{
		// ���Բ���
		// PushLayer(new ExampleLayer());		
		// 2D ��Ⱦ��׼��
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}
};

Spark::Application* Spark::CreateApplication()
{
	return new Sandbox();
}
