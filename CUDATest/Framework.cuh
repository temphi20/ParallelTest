#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <corecrt_math_defines.h>
#include <cstdio>
#include <string>

#include "../DigitalFilter.h"
#include "../Timer.h"

#define HEADER_SIZE 16
#define DATA1_SIZE 25011200
#define BLOCK_PER_THREAD 1024
#define THREAD_COUNT 200
#define SET_SIZE ( DATA1_SIZE / THREAD_COUNT )

#define TEST_FILE_NAME_1 "data0000000049_144919_872_0.bin"
#define TEST_FILE_NAME_2 "data0000000049_144919_872_1.bin"

typedef unsigned long long ULLONG;
typedef long long LLONG;

typedef unsigned long ULONG;
typedef long LONG;

typedef unsigned int UINT;
typedef int INT;

typedef unsigned short USHORT;
typedef short SHORT;

typedef char CHAR;
typedef bool BOOL;
typedef void VOID;

typedef uint8_t UINT8;