#include "framework.h"

int main()
{
	Test::Timer timer;
	timer.Start();
	timer.End();
	timer.Print();
	return 0;
}