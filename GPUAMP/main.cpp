#include "../ParallelTest/framework.h"
#include <amp.h>
using namespace concurrency;

//template<class T> unique

int main()
{
	unique_ptr<UINT> arr(new UINT(TEST_LNUM));
	unique_ptr<UINT> sum_arr(new UINT(TEST_LNUM));
	//ULLONG* arr = new ULLONG(TEST_LNUM);
	//ULLONG* sum_arr = new ULLONG(TEST_LNUM);
	//ULLONG arr[TEST_NUM] = {0};
	//ULLONG sum_arr[TEST_NUM];

	array_view<ULLONG, 1> sum(TEST_LNUM, sum_arr);
	sum.discard_data();
	
	parallel_for_each(
		sum.extent,
		[=](index<1> idx) restrict(amp)
		{
			sum_arr[idx];
			//sum_arr[idx] = arr[idx] + idx;
		}
	);


	system("pause");
	return 0;
}