#pragma once

#ifndef _StrategyTemplate_H_
#define _StrategyTemplate_H_

#include "DataStruct.h"
#include "TDEvent.h"
#include "Indicators.h"


class StrategyTemplate
{
public:
	StrategyTemplate(TDEvent*, Indicators*);
	~StrategyTemplate();

public:
	///初始化函数 不可传参
	///重写InIt若带传参 需单独执行InIt后再注册策略到mdevent(避免)
	void InIt();
	void ReceiveTick(TickInfomation);

	TickInfomation GetTick();
	void FunStart();
	bool FunState();
	
	char* InstrumentID();
	void FunEnd();

public:
	int LONG(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID);

	int SHORT(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID);

	int SELL(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID);

	int COVER(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID);


public:
	bool funend = true;
	TickInfomation __tick;
	char* __InstrumentID;

	TDEvent* tdevent;
	Indicators* indn;
};

#endif