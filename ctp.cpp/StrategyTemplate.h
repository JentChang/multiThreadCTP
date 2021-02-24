#pragma once

#ifndef _StrategyTemplate_H_
#define _StrategyTemplate_H_

#include "DataStruct.h"
#include "TDEvent.h"
#include "Indicators.h"


class StrategyTemplate
{
public:
	StrategyTemplate(TDEvent*);
	~StrategyTemplate();

public:
	///��ʼ������ ���ɴ���
	///��дInIt
	virtual void InIt();
	virtual void ReceiveTick(TickInfomation);
	virtual void OnBar(BarInfomation* bar);

	TickInfomation GetTick();
	void FunStart();
	bool FunState();
	
	char* InstrumentID();
	void FunEnd();

public:
	virtual
		CThostFtdcOrderField LONG(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID);

	virtual
		CThostFtdcOrderField SHORT(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID);

	virtual
		CThostFtdcOrderField SELL(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID);

	virtual
		CThostFtdcOrderField COVER(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID);


public:
	bool funend = true;
	TickInfomation __tick;
	char* __InstrumentID;

	TDEvent* tdevent;
	Indicators* indn;
};

#endif