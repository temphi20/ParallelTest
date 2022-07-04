//#include "../ParallelTest/framework.h"
//#include "CPUUsage.h"
#include "CPUMng.h"
#include "../ParallelTest/framework.h"
//#include <WinBase.h>

void use(WGSTest::CPUMng manager)
{
    int i;
    cin >> i;
    for (ULLONG i = 0; i < 2000000000000000ull; i++)
    {
        cout << "after: " << manager.GetCurrentProcessor() << endl;
    }
}

int main()
{
    WGSTest::CPUMng manager;

    //manager.GetUsage();
    //manager.Print();
    manager.SetProcessor(2);
    use(manager);
    
    //cout << "after: " << manager.GetCurrentProcessor() << endl;
    //cout << "after: " << manager.GetCurrentProcessor() << endl;
    //cout << "after: " << manager.GetCurrentProcessor() << endl;
    //cout << "after: " << manager.GetCurrentProcessor() << endl;
    //cout << "after: " << manager.GetCurrentProcessor() << endl;
    //cout << "after: " << manager.GetCurrentProcessor() << endl;
    //cout << "after: " << manager.GetCurrentProcessor() << endl;
    //cout << "after: " << manager.GetCurrentProcessor() << endl;
    //cout << "after: " << manager.GetCurrentProcessor() << endl;

    //manager.SetProcessor(9);
    //cout << "after: " << manager.GetCurrentProcessor() << endl;
    //cout << "after: " << manager.GetCurrentProcessor() << endl;
    //cout << "after: " << manager.GetCurrentProcessor() << endl;
    //cout << "after: " << manager.GetCurrentProcessor() << endl;
    //cout << "after: " << manager.GetCurrentProcessor() << endl;
    //cout << "after: " << manager.GetCurrentProcessor() << endl;
    //cout << "after: " << manager.GetCurrentProcessor() << endl;
    //cout << "after: " << manager.GetCurrentProcessor() << endl;
    //cout << "after: " << manager.GetCurrentProcessor() << endl;

    system("pause");
    
    return 0;
}
