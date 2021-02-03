#pragma once
#include "mainEvent.h"


class ReadNum
{
public:
	ReadNum(MEvent*);
	~ReadNum();

public:
	void read();


private:
	MEvent* __mevent;
};
