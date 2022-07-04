#include "CPUMng.h"

namespace WGSTest
{
	BOOL CPUMng::SetProcessor(DWORD iCore)
	{
		if (iCore >= m_nCore) return false;
		return SetProcessAffinityMask(GetCurrentProcess(), iCore + 1u);
	}

	/*void CPUMng::SetThread(UINT cpu)
	{
		if (cpu >= CPU_NUM) return;
		SetThreadAffinityMask(GetCurrentThread(), cpu + 1);
	}*/

	VOID CPUMng::GetUsage()
	{
		HQUERY hQuery;
		PDH_STATUS status = PdhOpenQuery(NULL, 0, &hQuery);
		if (isError(status)) return;
		
		for (size_t i = 0; i < m_nCore; i++)
		{
			HCOUNTER hCounter;
			TCHAR path[PDH_MAX_COUNTER_PATH] = { 0 };
			wsprintf(path, _T("\\Processor(%d)\\%% Processor Time"), (int)i);
			status = PdhValidatePath(path);
			if (isError(status))
			{
				m_usages.push_back(-4);
				continue;
			}

			status = PdhAddCounter(hQuery, path, 0, &hCounter);
			if (isError(status))
			{
				m_usages.push_back(-1);
				continue;
			}

			status = PdhCollectQueryData(hQuery);
			if (isError(status))
			{
				m_usages.push_back(-2);
				continue;
			}

			//Sleep(1000);
			
			DWORD dwType = 0;
			PDH_FMT_COUNTERVALUE fmtValue;
			status = PdhGetFormattedCounterValue(hCounter, PDH_FMT_DOUBLE | PDH_FMT_NOCAP100, &dwType, &fmtValue);
			if (isError(status))
			{
				isError(fmtValue.CStatus);
				m_usages.push_back(-3);
			}
			else 
				m_usages.push_back(fmtValue.doubleValue);
		}

		PdhCloseQuery(hQuery);
	}

	VOID CPUMng::GetCPUUsage()
	{
		//GetCPULoad();
	}

	VOID CPUMng::Reset()
	{
		vector<DOUBLE> empty;
		m_usages.swap(empty);
	}
}