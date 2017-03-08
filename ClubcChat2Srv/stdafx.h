#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <functional>
#include <algorithm>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <thread>
#include <mutex>
#include <string>
#include <vector>
#include <list>

#if defined(_WIN32) || defined(_WIN64) // Windows

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

inline void close(int sock)
{
	closesocket(sock);
}

#else // GCC

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iconv.h>
#include <assert.h>

inline void _itoa(int value, char *str, int radix)
{
	assert(radix == 10);
	sprintf(str, "%d", value);
}

#endif

