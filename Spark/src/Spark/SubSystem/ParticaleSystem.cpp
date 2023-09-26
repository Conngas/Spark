#include "spkpch.h"
#include "Spark/SubSystem/ParticaleSystem.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>
#include <random>

namespace Spark {

	class Random
	{
	public:
		static void Init()
		{
			s_RandomEngine.seed(std::random_device()());
		}

		// 归一化返回[0,1]的值
		static float Float()
		{
			return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
		}

	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};

	std::mt19937 Random::s_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

	ParticaleSystem::ParticaleSystem()
	{
		m_ParticalePool.resize(1000);
	}

	void ParticaleSystem::OnUpdate(Spark::Timestep ts)
	{
		for (auto& particale : m_ParticalePool)
		{
			if (!particale.Active)
				continue;
			if (particale.LifeRemaining <= 0.0f)
			{
				particale.Active = false;
				continue;
			}
			particale.LifeRemaining -= ts;
			particale.Position += particale.Velocity * (float)ts;
			particale.Rotation += 0.01f * (float)ts;
		}
	}

	void ParticaleSystem::OnRender(Spark::OrthographicCamera& camera)
	{
		Spark::Renderer2D::BeginScene(camera);
		for (auto& particale : m_ParticalePool)
		{
			if (!particale.Active)
				continue;
			float lifetime = particale.LifeRemaining / particale.LifeTime;
			glm::vec4 color = glm::lerp(particale.ColorBegin, particale.ColorEnd, lifetime);
			float size = glm::lerp(particale.SizeBegin, particale.SizeEnd, lifetime);
			Spark::Renderer2D::DrawRotationQuad(particale.Position, {size,size}, color, particale.Rotation);
		}
		Spark::Renderer2D::EndScene();
	}

	void ParticaleSystem::Emit(const ParticaleProps& particaleProps)
	{
		Particale& particale = m_ParticalePool[m_PoolIndex];
		particale.Active = true;
		particale.Position = particaleProps.Postion;
		particale.Rotation = Random::Float() * 2.0f * glm::pi<float>();
		// Velocity
		particale.Velocity = particaleProps.Velocity;
		particale.Velocity.x += particaleProps.VelocityVariation.x * (Random::Float() - 0.5f);
		particale.Velocity.y += particaleProps.VelocityVariation.y * (Random::Float() - 0.5f);
		// Color
		particale.ColorBegin = particaleProps.ColorBegin;
		particale.ColorEnd = particaleProps.ColorEnd;
		particale.LifeTime = particaleProps.LifeTime;
		particale.LifeRemaining = particaleProps.LifeTime;
		particale.SizeBegin = particaleProps.SizeBegine + particaleProps.SizeVariaion * Random::Float() - 0.5f;
		particale.SizeEnd = particaleProps.SizeEnd;

		m_PoolIndex = --m_PoolIndex % m_ParticalePool.size();
	}

}