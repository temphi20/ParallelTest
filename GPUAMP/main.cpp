#include "../ParallelTest/framework.h"
#include <cstdio>
#include <amp.h>
using namespace concurrency;

int main()
{
	WGSTest::Timer timer;
	UINT mask[6] = { 0, 1, 2, 3, 4, 5, };
	//UINT* arr_ptr = new UINT(TEST_LNUM);
	//unique_ptr<UINT> arr(new UINT(TEST_LNUM));
	vector<UINT> arr(TEST_LNUM);

	timer.Start();
	array_view<const UINT, 1> v_mask(6, mask);
	array_view<UINT, 1> v_arr(TEST_LNUM, arr);
	v_arr.discard_data();
	timer.End();
	timer.Print("array_view µî·Ï");
	//int a = 3;
	timer.Start();
	parallel_for_each(
		v_arr.extent,
		[=](index<1> idx) restrict(amp)
		{
			
			v_arr[idx] = v_mask[idx % 6] * 2;
			/*if (v_arr[idx])
			{
				v_arr[idx] = pow(v_arr[idx], 1.2);
			}*/

			//sum_arr[idx] = arr[idx] + idx;
		}
	);
	timer.End();
	timer.Print("parallel_for_each");

	//cout << v_arr[0] << endl;
	//cout << v_arr[1] << endl;
	//cout << v_arr[2] << endl;
	//cout << v_arr[3] << endl;
	//cout << v_arr[4] << endl;
	//cout << v_arr[5] << endl;
	//cout << v_arr[6] << endl;

	timer.Start();
	for (ULONG idx = 0; idx < TEST_LNUM; idx++)
	{
		//arr.get()[idx] = mask[idx % 6] * 2;
		arr[idx] = mask[idx % 6] * 2;
	}
	timer.End();
	timer.Print("for");

	//delete[] arr;

	/*cout << arr.get()[0] << endl;
	cout << arr.get()[1] << endl;
	cout << arr.get()[2] << endl;
	cout << arr.get()[3] << endl;
	cout << arr.get()[4] << endl;
	cout << arr.get()[5] << endl;
	cout << arr.get()[6] << endl;*/

	//cout << arr[0] << endl;
	//cout << arr[1] << endl;
	//cout << arr[2] << endl;
	//cout << arr[3] << endl;
	//cout << arr[4] << endl;
	//cout << arr[5] << endl;
	//cout << arr[6] << endl;


	system("pause");
	return 0;
}