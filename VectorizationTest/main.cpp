#include "../ParallelTest/framework.h"
#include <intrin.h>

bool EnableSSEInstructions()
{
	int CPUInfo[4] = { -1, };
	__cpuid(CPUInfo, 1);
	return CPUInfo[3] >> 24 & 0x1;
}

int main()
{
	cout << EnableSSEInstructions() << endl;

	system("pause");
	return 0;
}