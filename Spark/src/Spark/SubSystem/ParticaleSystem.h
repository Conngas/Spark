#pragma once
#include <Spark.h>

namespace Spark {

	struct ParticaleProps
	{
		glm::vec2 Postion;
		glm::vec2 Velocity, VelocityVariation;
		glm::vec4 ColorBegin, ColorEnd;
		float SizeBegine, SizeEnd, SizeVariaion;
		float LifeTime = 1.0f;
	};

	class ParticaleSystem
	{
	public:
		ParticaleSystem();

		void OnUpdate(Spark::Timestep ts);
		void OnRender(Spark::OrthographicCamera& camera);

		void Emit(const ParticaleProps& particaleProps);
	private:
		struct Particale
		{
			glm::vec2 Position;
			glm::vec2 Velocity;
			glm::vec4 ColorBegin, ColorEnd;
			float Rotation = 0.0f;
			float SizeBegin, SizeEnd;

			float LifeTime = 1.0f;
			float LifeRemaining = 0.0f;

			bool Active = false;
		};
		std::vector<Particale> m_ParticalePool;
		uint32_t m_PoolIndex = 999;
	};
}