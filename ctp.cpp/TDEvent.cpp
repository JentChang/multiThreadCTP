#include "TDEvent.h"


CtpTD* TDEvent::td;
//vector<TThostFtdcOrderSysIDType> TDEvent::sys_id_vtr;
//map<TThostFtdcOrderSysIDType, int> TDEvent::sys_id_time_map;
mutex TDEvent::sys_id_mutex;
mutex TDEvent::td_global_mutex;


TDEvent::TDEvent(CtpTD* td)
{
	TDEvent::td = td;
}

TDEvent::~TDEvent()
{
	this->td = nullptr;
}

CThostFtdcOrderField TDEvent::LONG(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID)
{
	CThostFtdcInputOrderField order;
	memset(&order, 0, sizeof(CThostFtdcInputOrderField));

	order.LimitPrice = price;
	order.VolumeTotalOriginal = volume;
	strcpy(order.InstrumentID, InstrumentID);

	order.Direction = THOST_FTDC_D_Buy;
	order.CombOffsetFlag[0] = THOST_FTDC_OF_Open;

	return this->SendOrder(order);
}

CThostFtdcOrderField TDEvent::SHORT(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID)
{
	CThostFtdcInputOrderField order;
	memset(&order, 0, sizeof(CThostFtdcInputOrderField));
	
	order.LimitPrice = price;
	order.VolumeTotalOriginal = volume;
	strcpy(order.InstrumentID, InstrumentID);

	order.Direction = THOST_FTDC_D_Sell;
	order.CombOffsetFlag[0] = THOST_FTDC_OF_Open;

	return this->SendOrder(order);
}

CThostFtdcOrderField TDEvent::SELL(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID, bool today)
{
	CThostFtdcInputOrderField order;
	memset(&order, 0, sizeof(CThostFtdcInputOrderField));

	order.LimitPrice = price;
	order.VolumeTotalOriginal = volume;
	strcpy(order.InstrumentID, InstrumentID);

	order.Direction = THOST_FTDC_D_Sell;
	if (today)
	{
		order.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
	}
	else {
		order.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
	}

	return this->SendOrder(order);
}

CThostFtdcOrderField TDEvent::COVER(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID, bool today)
{
	CThostFtdcInputOrderField order;
	memset(&order, 0, sizeof(CThostFtdcInputOrderField));
	order.LimitPrice = price;
	order.VolumeTotalOriginal = volume;
	strcpy(order.InstrumentID, InstrumentID);

	order.Direction = THOST_FTDC_D_Buy;
	if (today)
	{
		order.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
	}
	else {
		order.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
	}

	return this->SendOrder(order);
}

int TDEvent::ACTION(TThostFtdcOrderSysIDType sys_id, TThostFtdcExchangeIDType ExchangeID, TThostFtdcInstrumentIDType InstrumentID)
{
	CThostFtdcInputOrderActionField pInputOrderAction;
	memset(&pInputOrderAction, 0, sizeof(CThostFtdcInputOrderActionField));
	///OrderSysID FrontID+SessionID+OrderRef
	strcpy(pInputOrderAction.ExchangeID, ExchangeID);
	strcpy(pInputOrderAction.InstrumentID, InstrumentID);
	strcpy_s(pInputOrderAction.OrderSysID, sys_id);

	return TDEvent::td->ReqOrderAction(pInputOrderAction);;
}

CThostFtdcInvestorPositionField TDEvent::Postion(TThostFtdcExchangeIDType ExchangeID, TThostFtdcInstrumentIDType InstrumentID)
{
	TDEvent::td_global_mutex.lock();

	CThostFtdcQryInvestorPositionField pQryInvestorPosition;
	memset(&pQryInvestorPosition, 0, sizeof(CThostFtdcQryInvestorPositionField));

	strcpy(pQryInvestorPosition.ExchangeID, ExchangeID);
	strcpy(pQryInvestorPosition.ExchangeID, InstrumentID);

	CThostFtdcInvestorPositionField rtn_pos = TDEvent::td->ReqQryInvestorPosition(pQryInvestorPosition);
	TDEvent::td_global_mutex.lock();
	return rtn_pos;
}

CThostFtdcTradingAccountField TDEvent::Account()
{
	TDEvent::td_global_mutex.lock();
	CThostFtdcTradingAccountField rtn_acc = TDEvent::td->ReqQryTradingAccount();
	TDEvent::td_global_mutex.lock();
	return rtn_acc;
}

CThostFtdcOrderField TDEvent::SendOrder(CThostFtdcInputOrderField order)
{
	TDEvent::td_global_mutex.lock();
	CThostFtdcOrderField rtn_order;
	rtn_order = TDEvent::td->ReqOrderInsert(order);
	//TDEvent::sys_id_mutex.lock();
	//this->sys_id_vtr.push_back(rtn_order.OrderSysID);
	//this->sys_id_time_map[rtn_order.OrderSysID] = GetTickCount();
	//TDEvent::sys_id_mutex.unlock();
	TDEvent::td_global_mutex.unlock();
	return rtn_order;
}



//void TDEvent::ActionOrderThread(int mtime, TThostFtdcExchangeIDType ExchangeID, TThostFtdcInstrumentIDType InstrumentID)
//{
//	while (true)
//	{
//		TDEvent::sys_id_mutex.lock();
//		if (TDEvent::sys_id_vtr.size() == 0)
//		{
//			continue;
//		}
//		if (GetTickCount() - TDEvent::sys_id_time_map[TDEvent::sys_id_vtr[0]])
//		{
//			CThostFtdcInputOrderActionField pInputOrderAction;
//			memset(&pInputOrderAction, 0, sizeof(CThostFtdcInputOrderActionField));
//			strcpy(pInputOrderAction.ExchangeID, ExchangeID);
//			strcpy(pInputOrderAction.InstrumentID, InstrumentID);
//			strcpy_s(pInputOrderAction.OrderSysID, TDEvent::sys_id_vtr[0]);
//			TDEvent::td->ReqOrderAction(pInputOrderAction);
//			
//			map<TThostFtdcOrderSysIDType, int>::iterator key = TDEvent::sys_id_time_map.find(TDEvent::sys_id_vtr[0]);
//			if (key != TDEvent::sys_id_time_map.end())
//			{
//				TDEvent::sys_id_time_map.erase(key);
//			}
//			TDEvent::sys_id_vtr.erase(TDEvent::sys_id_vtr.begin());
//		}
//		TDEvent::sys_id_mutex.unlock();
//	}
//}


vector<CThostFtdcTradeField>* TDEvent::receiveTrade()
{
	return TDEvent::td->OnRtnTradeVtr();
}
