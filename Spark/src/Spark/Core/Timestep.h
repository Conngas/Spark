#pragma once
namespace Spark {
	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{

		}

		// 自动调用类型转换运算符，将 Timestep 实例转换为 float 类型，如 float a = timestem::st
		operator float() const { return m_Time; }

		float GetSeconds() { return m_Time; }
		float GetMilliSeconds() { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};
}