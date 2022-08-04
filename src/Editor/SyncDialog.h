#ifndef SYNC_DIALOG_H
#define SYNC_DIALOG_H

#include "Events/AcqEvent.h"

namespace BoxScore {

	class SyncDialog
	{
	public:
		SyncDialog();
		~SyncDialog();

		void OpenDialog() { m_open = true; }

		bool OnImGuiRender();
		AcqSyncArgsEvent GetSyncEvent() { return m_syncArgs; }

	private:
		bool m_open;
		SyncArgs m_syncArgs;
	};
}

#endif