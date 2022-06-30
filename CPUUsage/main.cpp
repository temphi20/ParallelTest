//#include "../ParallelTest/framework.h"
//#include "CPUUsage.h"
#include "CProcessor.h"
#include "../ParallelTest/framework.h"
//#include <WinBase.h>


int main()
{
    WGSTest::CProcessor manager;

    manager.SetProcessor(0);
    std::cout << "before: " << manager.GetCurrentProcessor() << std::endl;
    
    return 0;
}
