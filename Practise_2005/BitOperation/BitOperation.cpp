// BitOperation.cpp : �������̨Ӧ�ó������ڵ㡣
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

	return 0;
}

