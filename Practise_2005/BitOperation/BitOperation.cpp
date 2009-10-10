// BitOperation.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>


int _tmain(int argc, _TCHAR* argv[])
{
	unsigned int state = (0x20 | 0x40);

	std::cout << state << std::endl;

	state &= (~0x20);

	std::cout << state << std::endl;

	state &= (~0x40);

	std::cout << state << std::endl;

	int arrayTmp[] = {1, 2};
	int nValue = arrayTmp[3];

	return 0;
}

