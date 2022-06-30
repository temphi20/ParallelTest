#pragma once
#include <iostream>
#include <chrono>
#include <string>

namespace Test
{
	using namespace std;
	using namespace chrono;
	class Timer
	{
	public:
		Timer()
		{
		}
		~Timer(){}

		void Start();
		void End();
		void Reset();

		void Print();
		void Print(string task_name);
	private:
		system_clock::time_point timer;
		unique_ptr<duration<double>> duration_ptr;
	};
}

