
#pragma once
#include "mainEvent.h"


class WriteNum
{
public:
	WriteNum(MEvent*);
	~WriteNum();

public:
	void write();

private:
	MEvent* __mevent;
};

