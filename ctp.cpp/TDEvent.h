#pragma once
#ifndef _TDEvent_H_
#define _TDEvent_H_

#include "DataStruct.h"
#include "CtpTD.h"


class TDEvent
{
public:
	TDEvent(CtpTD*);
	~TDEvent();

public:
	int LONG(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID);
	int SHORT(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID);
	int SELL(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID, bool today=false);
	int COVER(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID, bool today=false);
	int ACTION(int OrderRef, TThostFtdcExchangeIDType ExchangeID, TThostFtdcInstrumentIDType InstrumentID);

	
private:
	int SendOrder(CThostFtdcInputOrderField order);
	static int ActionOrderThread(int time, TThostFtdcExchangeIDType ExchangeID, TThostFtdcInstrumentIDType InstrumentID);
	static vector<CThostFtdcTradeField>* receiveTrade();

private:
	//static vector<CThostFtdcInputOrderField> __order_arr;
	static CtpTD* td;

private:
	static bool __order_online;
	vector<CThostFtdcTradeField> __order_trade;
	static int __order_ref;
};

#endif