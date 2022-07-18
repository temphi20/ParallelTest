#include <fstream>
#include "Framework.cuh"

using namespace std;

UINT8 *result1 = new UINT8[DATA1_SIZE], *result2 = new UINT8[DATA1_SIZE];
BOOL isCudaError(cudaError_t status, std::string str);
cudaError_t cudaFilter(UINT8 *data);

/*// 1thread 당 200개의 연산(for문)
__global__ void addKernel(
	const UINT8* data, UINT8* data1, UINT8* data2,
	UINT8* res1, UINT8* res2,
	const UINT n_task1, const UINT n_block, const UINT n_thread)
{
	const ULONG taskIdx = blockIdx.x * n_thread + threadIdx.x;
	if (taskIdx >= n_task1) return;

	const double idt = 0.125, omega_c_1 = 2 * M_PI * 0.25, omega_c_2 = 2 * M_PI * 10;
	const double amplFac_1 = 1 / ((idt * omega_c_1 / 2) + 1),
		amplFac_2 = 1 / ((idt * omega_c_2 / 2) + 1),
		y1c_1 = (idt * omega_c_1 / 2) - 1,
		y1c_2 = (idt * omega_c_2 / 2) - 1,
		dt = idt;
	UINT8 output_1 = 0, output_2 = 0, x1_1 = 0, x1_2 = 0;

	for (UINT i = 0; i < SET_SIZE; i++)
	{
		const UINT realIdx = taskIdx * SET_SIZE + i;
		//printf("[%d] %d - %d", i, realIdx, taskIdx);
		data1[realIdx] = data[realIdx * 2];
		data2[realIdx] = data[realIdx * 2 + 1];

		output_1 = amplFac_1 * (data1[realIdx] - x1_1 - output_1 * y1c_1);
		x1_1 = data1[realIdx];
		output_2 = amplFac_2 * (data2[realIdx] - x1_2 - output_2 * y1c_2);
		x1_2 = data2[realIdx];

		res1[realIdx] = output_1;
		res2[realIdx] = output_2;
	}
}*/

// 1thread 당 200개의 연산(for문)
__global__ void addKernel(
	const UINT8* data, UINT8* data1, UINT8* data2,
	UINT8* res1, UINT8* res2)
{
	const ULONG taskIdx = threadIdx.x;

	const double idt = 0.125, omega_c_1 = 2 * M_PI * 0.25, omega_c_2 = 2 * M_PI * 10;
	const double amplFac_1 = 1 / ((idt * omega_c_1 / 2) + 1),
		amplFac_2 = 1 / ((idt * omega_c_2 / 2) + 1),
		y1c_1 = (idt * omega_c_1 / 2) - 1,
		y1c_2 = (idt * omega_c_2 / 2) - 1,
		dt = idt;
	UINT8 output_1 = 0, output_2 = 0, x1_1 = 0, x1_2 = 0;

	for (UINT i = 0; i < SET_SIZE; i++)
	{
		const UINT realIdx = taskIdx * SET_SIZE + i;
		data1[realIdx] = data[realIdx * 2];
		data2[realIdx] = data[realIdx * 2 + 1];

		output_1 = amplFac_1 * (data1[realIdx] - x1_1 - output_1 * y1c_1);
		x1_1 = data1[realIdx];
		output_2 = amplFac_2 * (data2[realIdx] - x1_2 - output_2 * y1c_2);
		x1_2 = data2[realIdx];

		res1[realIdx] = output_1;
		res2[realIdx] = output_2;
	}
}

int main()
{
	fstream fs1, fs2;
	fs1.open(TEST_FILE_NAME_1, ios::binary);
	fs2.open(TEST_FILE_NAME_2, ios::binary);

	UINT8* data = new UINT8[DATA1_SIZE * 2];
	for (size_t i = 0; i < DATA1_SIZE + 16; i++)
	{
		UINT8 tmp1, tmp2;
		fs1 >> tmp1;
		fs2 >> tmp2;
		if (i > 15)
		{
			data[i - 16] = tmp1;
			data[DATA1_SIZE + i - 16] = tmp2;
		}
	}
	fs1.close();
	fs2.close();

	cudaError_t status = cudaFilter(data);
	if (status != cudaSuccess) return 1;

	WGSTest::Timer timer;
	timer.Start();
	ofstream fout1("result1.bin", ios::out), fout2("result2.bin", ios::out);

	fout1.write(reinterpret_cast<CHAR *>(&result1[0]), DATA1_SIZE);
	fout2.write(reinterpret_cast<CHAR *>(&result2[0]), DATA1_SIZE);
	/*for (size_t i = 0; i < DATA1_SIZE + 16; i++)
	{
		fs1 << result1[i];
		fs2 << result2[i];
	}*/
	fout1.close();
	fout2.close();
	timer.End();
	timer.Print("file save");

	delete[] result1;
	delete[] result2;

	status = cudaDeviceReset();
	if (status != cudaSuccess) {
		fprintf(stderr, "cudaDeviceReset failed!\n");
		delete[] data;
		return 1;
	}

	delete[] data;
	return 0;
}

BOOL isCudaError(cudaError_t status, std::string str)
{
	BOOL ret = status != cudaSuccess;
	if (ret) fprintf(stderr, "%s: %s\n", str.c_str(), cudaGetErrorString(status));
	else printf("%s: sucess\n", str.c_str());

	return ret;
}

cudaError_t cudaFilter(UINT8 *data)
{
	UINT8 *dev_data = 0;
	UINT8 *dev_data1 = 0, *dev_data2 = 0;
	UINT8 *dev_res1 = 0, *dev_res2 = 0;
	const UINT n_data = DATA1_SIZE * 2u;

	WGSTest::Timer timer;
	cudaError_t status;


	status = cudaSetDevice(0);
	if (isCudaError(status, "set device")) goto Exit;

	// allocation
	timer.Start();
	status = cudaMalloc((void**)&dev_data, n_data * sizeof(UINT8));
	if (isCudaError(status, "data allocation")) goto Exit;
	status = cudaMalloc((void**)&dev_data1, DATA1_SIZE * sizeof(UINT8));
	if (isCudaError(status, "data1 allocation")) goto Exit;
	status = cudaMalloc((void**)&dev_data2, DATA1_SIZE * sizeof(UINT8));
	if (isCudaError(status, "data2 allocation")) goto Exit;
	status = cudaMalloc((void**)&dev_res1, DATA1_SIZE * sizeof(UINT8));
	if (isCudaError(status, "result1 allocation")) goto Exit;
	status = cudaMalloc((void**)&dev_res2, DATA1_SIZE * sizeof(UINT8));
	if (isCudaError(status, "result1 allocation")) goto Exit;
	timer.End();
	timer.Print("data, data1, data2, result1, result2 malloc");

	// memory copy
	timer.Start();
	status = cudaMemcpy(dev_data, data, n_data * sizeof(UINT8), cudaMemcpyHostToDevice);
	if (isCudaError(status, "data memory copy")) goto Exit;
	timer.End();
	timer.Print("data memcpy");

	// calculate block
	timer.Start();
	addKernel << <1, THREAD_COUNT >> > (dev_data, dev_data1, dev_data2, dev_res1, dev_res2);

	// ULONG n_block = DATA1_SIZE / BLOCK_PER_THREAD + 1;
	/*const UINT n_task1 = (DATA1_SIZE / SET_SIZE) + (DATA1_SIZE % SET_SIZE); // 125,056개 작업
	const UINT n_thread = 512; // 한번 too many resources requested for launch 떠서 1블록당 스레드 갯수를 최대갯수에서 반으로 줄임
	const UINT n_block = ceil((double)n_task1 / (double)n_thread);
	//const ULONG n_task_per_thread = BLOCK_PER_THREAD / SET_SIZE;
	cout << "data1 task cnt: " << n_task1 << endl;
	cout << "block cnt: " << n_block << endl;
	//cout << "task cnt per thread: " << n_task_per_thread << endl;

	//const ULONG n_block = DATA1_SIZE / BLOCK_PER_THREAD;
	addKernel << <n_block, n_thread >> > (dev_data, dev_data1, dev_data2, dev_res1, dev_res2, n_task1, n_block, n_thread);*/

	// kernel error
	status = cudaGetLastError();
	if (isCudaError(status, "add kernel")) goto Exit;

	status = cudaDeviceSynchronize();
	if (isCudaError(status, "device synchronize")) goto Exit;

	timer.End();
	timer.Print("calculation 완료");
	
	
	// Copy output vector from GPU buffer to host memory.
	timer.Start();
	status = cudaMemcpy(result1, dev_res1, DATA1_SIZE * sizeof(UINT8), cudaMemcpyDeviceToHost);
	if (isCudaError(status, "result1 memcpy")) goto Exit;
	status = cudaMemcpy(result2, dev_res2, DATA1_SIZE * sizeof(UINT8), cudaMemcpyDeviceToHost);
	if (isCudaError(status, "result2 memcpy")) goto Exit;
	timer.End();
	timer.Print("result memcpy gpu 2 cpu");

Exit:
	cudaFree(dev_data1);
	cudaFree(dev_data2);
	cudaFree(dev_res1);
	cudaFree(dev_res2);

	return status;
}