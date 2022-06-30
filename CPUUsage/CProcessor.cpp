#include "CProcessor.h"

namespace WGSTest
{
	void CProcessor::SetProcessor(UINT cpu)
	{
		if (cpu >= CPU_NUM) return;
		SetProcessAffinityMask(GetCurrentProcess(), cpu + 1);
	}

	void CProcessor::SetThread(UINT cpu)
	{
		if (cpu >= CPU_NUM) return;
		SetThreadAffinityMask(GetCurrentThread(), cpu + 1);
	}
}