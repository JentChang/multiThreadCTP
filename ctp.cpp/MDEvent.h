#pragma once
#ifndef _MDEvent_H_
#define _MDEvent_H_

#include "DataStruct.h"
#include "StrategyTemplate.h"


class MDEvent
{
public:
	MDEvent();
	~MDEvent();

public:

	void InsNum(int);
	int AddTick(TickInfomation);

	void AddStrategy(StrategyTemplate*);
	void StrategyStart();
	void join();

private:
	static void SendTickThreadFun(StrategyTemplate*);

private:
	static vector<TickInfomation> __tick_vtr;
	static vector<StrategyTemplate*> strategy_vtr;
	vector<std::thread> thread_vtr;
	static mutex mtx;

};


#endif