#pragma once
namespace Spark {
	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{

		}

		// �Զ���������ת����������� Timestep ʵ��ת��Ϊ float ���ͣ��� float a = timestem::st
		operator float() const { return m_Time; }

		float GetSeconds() { return m_Time; }
		float GetMilliSeconds() { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};
}