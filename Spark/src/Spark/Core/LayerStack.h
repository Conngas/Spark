#pragma once

#include "Spark/Core/Core.h"
#include "Spark/Core/Layer.h"

namespace Spark {
	
	class SPARK_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		// ÐÂ½¨Óë¸²¸Ç
		void PushLayer(Layer* layer);
		void PushOverLay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverLay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}
