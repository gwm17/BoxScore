#ifndef SCALAR_PANEL_H
#define SCALAR_PANEL_H

#include "Core/ScalarDistributor.h"
#include "DAQ/DigitizerDefs.h"

namespace BoxScore {

	class ScalarPanel
	{
	public:
		ScalarPanel();
		~ScalarPanel();

		void ResetScalars() { ScalarDistributor::ClearScalars();  m_elapsedTime = 0.0; }

		void OnImGuiRender(const std::vector<DigitizerArgs>& argList, double timestep);

	private:
		DigitizerArgs m_selectedArgs;
		std::vector<std::string> m_scalarNames;
		double m_elapsedTime;
	};

}

#endif