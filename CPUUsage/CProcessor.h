#pragma once
#include <iostream>
#include <thread>
#include <Windows.h> // minwinbase.h에서 PCONTEXT is not define 에러 떠서 추가
#include <WinBase.h>

namespace WGSTest
{
	//using namespace std;
	class CProcessor
	{
	public:
		CProcessor()
		{
			CPU_NUM = std::thread::hardware_concurrency();
			std::cout << "CPU 갯수: " << CPU_NUM << std::endl;
		}
		~CProcessor()
		{}

		void SetProcessor(UINT cpu);
		void SetThread(UINT cpu);
		inline UINT GetCurrentProcessor() { return GetCurrentProcessorNumber(); }
	private:
		UINT CPU_NUM;
	};
}


