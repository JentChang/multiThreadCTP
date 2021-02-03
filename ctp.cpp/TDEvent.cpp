#include "TDEvent.h"


CtpTD* TDEvent::td;
bool TDEvent::__order_online;
int TDEvent::__order_ref;

TDEvent::TDEvent(CtpTD* td)
{
	TDEvent::td = td;
	this->__order_online = false;
}

TDEvent::~TDEvent()
{
	this->td = nullptr;
}

int TDEvent::LONG(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID)
{
	this->__order_online = true;
	CThostFtdcInputOrderField order;
	order.LimitPrice = price;
	order.VolumeTotalOriginal = volume;
	strcpy(order.InstrumentID, InstrumentID);

	order.Direction = THOST_FTDC_D_Buy;
	order.CombOffsetFlag[0] = THOST_FTDC_OF_Open;

	return this->SendOrder(order);
}

int TDEvent::SHORT(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID)
{
	this->__order_online = true;
	CThostFtdcInputOrderField order;
	order.LimitPrice = price;
	order.VolumeTotalOriginal = volume;
	strcpy(order.InstrumentID, InstrumentID);

	order.Direction = THOST_FTDC_D_Sell;
	order.CombOffsetFlag[0] = THOST_FTDC_OF_Open;

	return this->SendOrder(order);
}

int TDEvent::SELL(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID, bool today)
{
	this->__order_online = true;
	CThostFtdcInputOrderField order;
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

int TDEvent::COVER(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID, bool today)
{
	this->__order_online = true;
	CThostFtdcInputOrderField order;
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

int TDEvent::ACTION(int OrderRef, TThostFtdcExchangeIDType ExchangeID, TThostFtdcInstrumentIDType InstrumentID)
{
	CThostFtdcInputOrderActionField pInputOrderAction;
	///OrderSysID FrontID+SessionID+OrderRef
	sprintf(pInputOrderAction.OrderRef, "%d", OrderRef);
	strcpy(pInputOrderAction.ExchangeID, ExchangeID);
	strcpy(pInputOrderAction.InstrumentID, InstrumentID);
	this->__order_ref = 0;
	this->__order_online = false;
	return TDEvent::td->ReqOrderAction(pInputOrderAction);;
}

int TDEvent::SendOrder(CThostFtdcInputOrderField order)
{
	this->__order_online = true;
	this -> __order_ref = TDEvent::td->ReqOrderInsert(order);

	return this->__order_ref;
}

int TDEvent::ActionOrderThread(int time, TThostFtdcExchangeIDType ExchangeID, TThostFtdcInstrumentIDType InstrumentID)
{
	TThostFtdcOrderRefType ref;
	sprintf(ref, "%d", TDEvent::__order_ref);
	vector<CThostFtdcTradeField>* onRtnTradeVtr = TDEvent::receiveTrade();

	Sleep(time);
	//for (size_t i = 0; i < onRtnTradeVtr->size(); i++)
	//{
	//	CThostFtdcTradeField onRtnTrade = onRtnTradeVtr->at[i];
	//	
	//	if (strcmp(onRtnTrade.OrderRef, ref) == 0)
	//	{
	//		onRtnTradeVtr->erase(onRtnTradeVtr->at[i]);
	//		return 0;
	//	}
	//}

	CThostFtdcInputOrderActionField pInputOrderAction;
	sprintf(pInputOrderAction.OrderRef, "%d", ref);
	strcpy(pInputOrderAction.ExchangeID, ExchangeID);
	strcpy(pInputOrderAction.InstrumentID, InstrumentID);
	TDEvent::__order_ref = 0;
	TDEvent::__order_online = false;
	return TDEvent::td->ReqOrderAction(pInputOrderAction);
}


vector<CThostFtdcTradeField>* TDEvent::receiveTrade()
{
	return TDEvent::td->OnRtnTradeVtr();
}
