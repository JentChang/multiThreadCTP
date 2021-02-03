#pragma once
#include "read.h"
#include <iostream>

ReadNum::ReadNum(MEvent* mevent)
{
	this->__mevent = mevent;
}

ReadNum::~ReadNum()
{
}

void ReadNum::read()
{
	while (true)
	{
		int i = this->__mevent->num();
		if (i != -1)
		{
			std::cout << i << std::endl;
			this->__mevent->subnum();
		}
	}
}
