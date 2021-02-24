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
	CThostFtdcOrderField LONG(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID);
	CThostFtdcOrderField SHORT(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID);
	CThostFtdcOrderField SELL(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID, bool today=false);
	CThostFtdcOrderField COVER(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID, bool today=false);
	int ACTION(TThostFtdcOrderSysIDType sys_id, TThostFtdcExchangeIDType ExchangeID, TThostFtdcInstrumentIDType InstrumentID);

	///查询持仓
	CThostFtdcInvestorPositionField Postion(TThostFtdcExchangeIDType ExchangeID, TThostFtdcInstrumentIDType InstrumentID);
	///查询账户资金
	CThostFtdcTradingAccountField Account();

	
private:
	CThostFtdcOrderField SendOrder(CThostFtdcInputOrderField order);
	//static void ActionOrderThread(int mtime, TThostFtdcExchangeIDType ExchangeID, TThostFtdcInstrumentIDType InstrumentID);
	static vector<CThostFtdcTradeField>* receiveTrade();

private:
	//static vector<CThostFtdcInputOrderField> __order_arr;
	static CtpTD* td;

private:
	vector<CThostFtdcTradeField> __order_trade;
	static mutex sys_id_mutex;
	//static vector<TThostFtdcOrderSysIDType> sys_id_vtr;
	//static map<TThostFtdcOrderSysIDType, int> sys_id_time_map;

	static mutex td_global_mutex;
};

#endif