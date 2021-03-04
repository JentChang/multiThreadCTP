// test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <mutex>
#include <math.h>

#include <cstring>
#include <fstream>
#include <queue>
#include <map>
#include <Windows.h>
#include <tchar.h>

using namespace std;

#include "time.h"
#include "stdio.h"
int main(void)
{
	bool IsTradingTime = false;
	bool IsStart = false;
	while (true)
	{
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		if (
			(sys.wHour >= 0 && sys.wHour < 8) ||
			(sys.wHour >= 16 && sys.wHour < 20) ||
			(sys.wHour == 8 && sys.wMinute <= 31) ||
			(sys.wHour == 20 && sys.wMinute <= 31)
			)
		{
			IsTradingTime = false;
		}
		else {
			IsTradingTime = true;
		}

		if (IsTradingTime)
		{
			if (!IsStart)
			{
				std::cout << sys.wHour << sys.wMinute << endl;
				HINSTANCE handle = ShellExecute(NULL, _T("open"), _T("cpp.exe"), NULL, NULL, SW_SHOWNORMAL);
				IsStart = true;
				std::cout << "cpp.exe has started" << endl;
			}
		}
		else {
			if (IsStart)
			{
				std::cout << sys.wHour << sys.wMinute << endl;
				system(_T("taskkill /F /IM cpp.exe /T"));
				IsStart = false;
				std::cout << "cpp.exe has taskkilled" << endl;
			}	
		}
		Sleep(1000);
	}
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
