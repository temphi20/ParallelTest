#include "Timer.h"

namespace WGSTest
{
	void Timer::Start()
	{
		timer = system_clock::now();
	}
	
	void Timer::End()
	{
		Reset();
		duration_ptr = make_unique<duration<double>>(system_clock::now() - timer);
	}
	
	void Timer::Reset()
	{
		duration_ptr.reset();
	}
	
	void Timer::Print()
	{
		cout << duration_ptr->count() << " sec" << endl;
	}

	void Timer::Print(string task_name)
	{
		cout << task_name << " / ";
		Print();
	}
}