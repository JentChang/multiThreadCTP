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
	int AddTick(TickInfomation);

	void AddStrategy(StrategyTemplate*);
	void StrategyStart();
	void StrategyEnd();

private:
	static void SendTickThreadFun(StrategyTemplate*);

public:
	vector<std::thread> thread_vtr;

private:
	static map<string, TickInfomation> __tick_map;

	static bool ISWRITING;
	static int ISREADING;

	static vector<StrategyTemplate*> strategy_vtr;
	
	static bool threadclose;
};


#endif