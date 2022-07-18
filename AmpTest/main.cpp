#include <iostream>
#include <fstream>
#include <amp.h>
#include <vector>
#include <corecrt_math_defines.h>

#include "../ParallelTest/Timer.h"
#include "../ParallelTest/DigitalFilter.h"



#define HEADER_SIZE 16
const unsigned int DATA1_SIZE = 25011200;
const unsigned int THREAD_COUNT = 200;
const unsigned int SET_SIZE = DATA1_SIZE / THREAD_COUNT;

#define TEST_FILE_NAME_1 "data0000000049_144919_872_0.bin"
#define TEST_FILE_NAME_2 "data0000000049_144919_872_1.bin"

using namespace std;
using namespace concurrency;

int main()
{
	WGSTest::Timer timer;
	vector<UINT> zero(THREAD_COUNT);
	vector<UINT> data1(DATA1_SIZE), data2(DATA1_SIZE), res1(DATA1_SIZE), res2(DATA1_SIZE);
	//UINT8* data1 = new UINT8[DATA1_SIZE], * data2 = new UINT8[DATA1_SIZE];
	//UINT8* res1 = new UINT8[DATA1_SIZE], * res2 = new UINT8[DATA1_SIZE];

	ifstream fin1(TEST_FILE_NAME_1, ios::binary), fin2(TEST_FILE_NAME_2, ios::binary);
	for (size_t i = 0; i < DATA1_SIZE + HEADER_SIZE; i++)
	{
		UINT8 tmp1, tmp2;
		fin1 >> tmp1;
		fin2 >> tmp2;
		if (i > 15)
		{
			data1[i - 16] = tmp1;
			data2[i - 16] = tmp2;
		}
	}
	fin1.close();
	fin2.close();

	timer.Start();
	array_view<UINT, 1> v_zero(THREAD_COUNT, zero);
	array_view<const UINT, 1> v_data1(DATA1_SIZE, data1), v_data2(DATA1_SIZE, data2);
	array_view<UINT, 1> v_res1(DATA1_SIZE, res1), v_res2(DATA1_SIZE, res2);
	v_zero.discard_data();
	v_res1.discard_data();
	v_res2.discard_data();
	timer.End();
	timer.Print("create array_view");
	

	timer.Start();
	parallel_for_each(
		v_zero.extent,
		[=](index<1> idx) restrict(amp)
		{
			const double idt = 0.125, omega_c_1 = 2 * M_PI * 0.25, omega_c_2 = 2 * M_PI * 10;
			const double amplFac_1 = 1 / ((idt * omega_c_1 / 2) + 1),
						 amplFac_2 = 1 / ((idt * omega_c_2 / 2) + 1),
						 y1c_1 = (idt * omega_c_1 / 2) - 1,
						 y1c_2 = (idt * omega_c_2 / 2) - 1,
						 dt = idt;
			unsigned int output_1 = 0, output_2 = 0, x1_1 = 0, x1_2 = 0;

			for (UINT i = 0; i < SET_SIZE; i++)
			{
				const auto realIdx = idx * SET_SIZE + i;

				output_1 = amplFac_1 * (v_data1[realIdx] - x1_1 - output_1 * y1c_1);
				x1_1 = v_data1[realIdx];
				output_2 = amplFac_2 * (v_data2[realIdx] - x1_2 - output_2 * y1c_2);
				x1_2 = v_data2[realIdx];

				v_res1[realIdx] = output_1;
				v_res2[realIdx] = output_2;
			}
			
		}
	);
	timer.End();
	timer.Print("parallel_for_each");



	ofstream fout1("result1.bin", ios::out), fout2("result2.bin", ios::out);
	fout1.write(reinterpret_cast<CHAR*>(&res1[0]), DATA1_SIZE);
	fout2.write(reinterpret_cast<CHAR*>(&res2[0]), DATA1_SIZE);
	fout1.close();
	fout2.close();

	//timer.Start();
	//for (ULONG idx = 0; idx < TEST_LNUM; idx++)
	//{
		//arr.get()[idx] = mask[idx % 6] * 2;
		//arr[idx] = mask[idx % 6] * 2;
	//}
	//timer.End();
	//timer.Print("for");


	//delete[] data1, data2;

	system("pause");
	return 0;
}