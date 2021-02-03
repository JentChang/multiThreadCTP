#pragma once
#include "mainEvent.h"

MEvent::MEvent()
{
}

MEvent::~MEvent()
{
}

int MEvent::addnum(int num)
{
	if (this->__num.size() >= 9999)
	{
		return 1;
	}
	this->__num.push_back(num);
	return 0;
}

void MEvent::subnum()
{
	this->__num.erase(this->__num.begin());
}

int MEvent::num()
{
	if (this->__num.size() == 0)
	{
		return -1;
	}
	return this->__num.at(0);
}
