#pragma once
#include <iostream>
#include <thread>
#include <Windows.h> // minwinbase.h���� PCONTEXT is not define ���� ���� �߰�
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
			std::cout << "CPU ����: " << CPU_NUM << std::endl;
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


