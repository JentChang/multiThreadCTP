#pragma once
#include "write.h"
#include <Windows.h>
#include <iostream>


WriteNum::WriteNum(MEvent* mevent)
{
	this->__mevent = mevent;
}

WriteNum::~WriteNum()
{
}

void WriteNum::write()
{
	int i = 0;
	while (i <= 10)
	{
		int addres = this->__mevent->addnum(i);
		if (addres != 0)
		{
			std::cout << "mevent->addnum error " << addres << std::endl;
		}
		i++;

		Sleep(500);
	}
}
