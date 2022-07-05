#pragma once
#define _HAS_STD_BYTE 0
#include <iostream>
#include "Timer.h"
using namespace std;

typedef double DOUBLE;
typedef int INT;
typedef long LONG;
typedef long long int LLONG;

typedef unsigned int UINT;
typedef unsigned long ULONG;
typedef unsigned long long int ULLONG;


typedef unsigned char byte;

const UINT THREAD_NUM = 10;
const UINT TEST_NUM = 100000000;
const ULONG TEST_LNUM = 4294967295;
const ULLONG TEST_LLNUM = 18446744073709551615;

