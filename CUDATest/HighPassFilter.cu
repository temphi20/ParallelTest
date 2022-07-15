#include "Framework.cuh"

__global__ void addKernel(
	const UINT8* data, UINT8* data1, UINT8* data2,
	UINT8* res1, UINT8* res2)
{
	const double idt = 0.125, omega_c_1 = 2 * M_PI * 0.25, omega_c_2 = 2 * M_PI * 10;
	const double amplFac_1 = 1 / ((idt * omega_c_1 / 2) + 1),
			  	 amplFac_2 = 1 / ((idt * omega_c_2 / 2) + 1),
				 y1c_1 = (idt * omega_c_1 / 2) - 1,
				 y1c_2 = (idt * omega_c_2 / 2) - 1,
				 dt = idt;
	UINT8 output_1 = 0, output_2 = 0, x1_1 = 0, x1_2 = 0;
	
	const ULONG i = blockIdx.x * BLOCK_PER_THREAD + threadIdx.x;
	
	data1[i] = data[i * 2];
	data2[i] = data[i * 2 + 1];

	output_1 = amplFac_1 * (data1[i] - x1_1 - output_1 * y1c_1);
	x1_1 = data1[i];
	output_2 = amplFac_2 * (data2[i] - x1_2 - output_2 * y1c_2);
	x1_2 = data2[i];

	res1[i] = output_1;
	res2[i] = output_2;
}

BOOL isCudaError(cudaError_t status, std::string str = "")
{
	BOOL ret = status != cudaSuccess;
	if (ret) fprintf(stderr, "%s: %s\n", str, cudaGetErrorString(status));
	else printf("%s: sucess\n", str);

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
	if (isCudaError(status)) goto Exit;

	// allocation
	status = cudaMalloc((void**)&dev_data, n_data * sizeof(UINT8));
	if (isCudaError(status)) goto Exit;
	status = cudaMalloc((void**)&dev_data1, DATA1_SIZE * sizeof(UINT8));
	if (isCudaError(status)) goto Exit;
	status = cudaMalloc((void**)&dev_data2, DATA1_SIZE * sizeof(UINT8));
	if (isCudaError(status)) goto Exit;
	status = cudaMalloc((void**)&dev_res1, DATA1_SIZE * sizeof(UINT8));
	if (isCudaError(status)) goto Exit;
	status = cudaMalloc((void**)&dev_res2, DATA1_SIZE * sizeof(UINT8));
	if (isCudaError(status)) goto Exit;

	// memory copy
	status = cudaMemcpy(dev_data, data, n_data * sizeof(UINT8), cudaMemcpyHostToDevice);
	if (isCudaError(status)) goto Exit;

	// calculate block
	ULONG n_block = DATA1_SIZE / BLOCK_PER_THREAD + 1;
	HighPassFilter filter1(0.125, 2 * M_PI * 0.25), filter2(0.125, 2 * M_PI * 10);
	addKernel <<<n_block, BLOCK_PER_THREAD >>> (dev_data, dev_data1, dev_data2, dev_res1, dev_res2);

	// kernel error
	status = cudaGetLastError();
	if (isCudaError(status)) goto Exit;

	status = cudaDeviceSynchronize();
	if (isCudaError(status)) goto Exit;

	timer.End();
	timer.Print("cal ¿Ï·á");

	// Copy output vector from GPU buffer to host memory.
	//status = cudaMemcpy(arr, dev_arr, ARRAY_SIZE * sizeof(ULLONG), cudaMemcpyDeviceToHost);
	//if (isCudaError(status)) goto Exit;

Exit:
	cudaFree(dev_data1);
	cudaFree(dev_data2);
	cudaFree(dev_res1);
	cudaFree(dev_res2);

	return status;
}