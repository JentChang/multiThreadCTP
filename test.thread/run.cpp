#pragma once
#include "write.h"
#include "read.h"
#include <thread>

WriteNum* wn;
ReadNum* rn;
MEvent* me;

void twn_fun()
{
	wn->write();
}

void trn_fun()
{
	rn->read();
}

int main()
{
	me = new MEvent();
	wn = new WriteNum(me);
	rn = new ReadNum(me);

	std::thread twn(twn_fun);
	std::thread trn(trn_fun);

	twn.join();
	trn.join();
}