#include <iostream>
#include <vector>
//#include <thread>
#include <Windows.h> // minwinbase.h에서 PCONTEXT is not define 에러 떠서 추가
#include <WinBase.h>
#include <Pdh.h>
#include <tchar.h>

#pragma comment(lib, "pdh.lib")

namespace WGSTest
{
	using namespace std;
	class CPUMng
	{
	public:
		CPUMng()
		{
			
			GetSystemInfo(&m_sInfo);
			m_nCore = m_sInfo.dwNumberOfProcessors;
			//CPU_NUM = GetSystemInfo();
			//m_nCore = std::thread::hardware_concurrency();
			cout << "CORE 갯수: " << m_nCore << endl;
		}
		~CPUMng()
		{}

		BOOL SetProcessor(DWORD cpu);
		//void SetThread(UINT cpu);
		VOID GetUsage();
		VOID Reset();
		VOID GetCPUUsage();
		
		inline DWORD GetCurrentProcessor() { return GetCurrentProcessorNumber(); }
		inline VOID Print()
		{
			for (size_t i = 0; i < m_usages.size(); i++)
			{
				cout << "CORE " << i << " : " << m_usages[i] << "%" << endl;
			}
		}
	private:
		SYSTEM_INFO m_sInfo;
		DWORD m_nCore;
		//vector<HCOUNTER> m_hCounters;
		vector<DOUBLE> m_usages;
		inline BOOL isError(PDH_STATUS status)
		{
			BOOL ret = status != ERROR_SUCCESS;
			if(ret) wprintf(L"Error 0x%x\n", status);
			return ret;
		}
	};
}


