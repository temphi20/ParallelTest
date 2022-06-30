#include "CPUMng.h"

namespace WGSTest
{
	BOOL CPUMng::SetProcessor(DWORD iCore)
	{
		if (iCore >= m_nCore) return false;
		return SetProcessAffinityMask(GetCurrentProcess(), iCore + 1ul);
	}

	/*void CPUMng::SetThread(UINT cpu)
	{
		if (cpu >= CPU_NUM) return;
		SetThreadAffinityMask(GetCurrentThread(), cpu + 1);
	}*/

	void CPUMng::GetUsage()
	{
		HQUERY hQuery;
		PDH_STATUS status = PdhOpenQuery(NULL, 0, &hQuery);
		if (isError(status)) return;

		
		for (size_t i = 0; i < m_nCore; i++)
		{	
			HCOUNTER phCounter;
			TCHAR path[1024] = {0};
			wsprintf(path, _T("\\Processor(%d)\\%% Processor Time"), (int)i);
			status = PdhAddCounter(hQuery, path, 0, &phCounter);
			if (!isError(status)) m_hCounters.push_back(phCounter);
		}
	}
}