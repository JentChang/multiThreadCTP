#include "CtpMD.h"



CtpMD::CtpMD(CThostFtdcMdApi* api, CThostFtdcReqUserLoginField* pUserLogin, MDEvent* mdevent):
	api(api), userLogin(pUserLogin), mdevent(mdevent)
{
	requestID = 1;
}


CtpMD::~CtpMD()
{
	if (mdevent)
	{
		mdevent = nullptr;
	}
	if (api)
	{
		api->RegisterSpi(nullptr);
		api->Release();
		api = nullptr;
	}
}


void CtpMD::OnFrontConnected()
{
	std::cout << "OnFrontConnected..." << std::endl;
	if (userLogin)
	{
		api->ReqUserLogin(userLogin, requestID++);
	}
}


void CtpMD::OnFrontDisconnected(int nReason)
{
	std::cout << "OnFrontDisconnected, nReason:"<< nReason << std::endl;
}


void CtpMD::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		std::cout << "OnRspUserLogin ok"<< std::endl;
	}
	else {
		std::cout << "OnRspUserLogin error:" 
			<< pRspInfo->ErrorID 
			<< pRspInfo->ErrorMsg
			<< std::endl;
	}
}


///错误应答
void CtpMD::OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "OnRspError:"
		<< pRspInfo->ErrorID
		<< pRspInfo->ErrorMsg
		<< std::endl;
}

///订阅行情应答
void CtpMD::OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		std::cout << "OnRspSubMarketData ok" << std::endl;
	}
	else {
		std::cout << "OnRspSubMarketData error:"
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
	}
}


///深度行情通知
void CtpMD::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
	TickInfomation tick_info;
	tick_info.EMPTY = false;
	///交易日
	strcpy(tick_info.TradingDay, pDepthMarketData->TradingDay);
	///合约代码
	strcpy(tick_info.InstrumentID, pDepthMarketData->InstrumentID);
	///交易所代码
	strcpy(tick_info.ExchangeID, pDepthMarketData->ExchangeID);
	///合约在交易所的代码
	strcpy(tick_info.ExchangeInstID, pDepthMarketData->ExchangeInstID);
	///最新价
	tick_info.LastPrice = pDepthMarketData->LastPrice;
	///上次结算价
	tick_info.PreSettlementPrice = pDepthMarketData->PreSettlementPrice;
	///昨收盘
	tick_info.PreClosePrice = pDepthMarketData->PreClosePrice;
	///昨持仓量
	tick_info.PreOpenInterest = pDepthMarketData->PreOpenInterest;
	///今开盘
	tick_info.OpenPrice = pDepthMarketData->OpenPrice;
	///最高价
	tick_info.HighestPrice = pDepthMarketData->HighestPrice;
	///最低价
	tick_info.LowestPrice = pDepthMarketData->LowestPrice;
	///数量
	tick_info.Volume = pDepthMarketData->Volume;
	///成交金额
	tick_info.Turnover = pDepthMarketData->Turnover;
	///持仓量
	tick_info.OpenInterest = pDepthMarketData->OpenInterest;
	///今收盘
	tick_info.ClosePrice = pDepthMarketData->ClosePrice;
	///本次结算价
	tick_info.SettlementPrice = pDepthMarketData->SettlementPrice;
	///涨停板价
	tick_info.UpperLimitPrice = pDepthMarketData->UpperLimitPrice;
	///跌停板价
	tick_info.LowerLimitPrice = pDepthMarketData->LowerLimitPrice;
	///昨虚实度
	tick_info.PreDelta = pDepthMarketData->PreDelta;
	///今虚实度
	tick_info.CurrDelta = pDepthMarketData->CurrDelta;
	///最后修改时间
	strcpy(tick_info.UpdateTime, pDepthMarketData->UpdateTime);
	///最后修改毫秒
	tick_info.UpdateMillisec = pDepthMarketData->UpdateMillisec;
	///申买价一
	tick_info.BidPrice1 = pDepthMarketData->BidPrice1;
	///申买量一
	tick_info.BidVolume1 = pDepthMarketData->BidVolume1;
	///申卖价一
	tick_info.AskPrice1 = pDepthMarketData->AskPrice1;
	///申卖量一
	tick_info.AskVolume1 = pDepthMarketData->AskVolume1;
	///申买价二
	tick_info.BidPrice2 = pDepthMarketData->BidPrice2;
	///申买量二
	tick_info.BidVolume2 = pDepthMarketData->BidVolume2;
	///申卖价二
	tick_info.AskPrice2 = pDepthMarketData->AskPrice2;
	///申卖量二
	tick_info.AskVolume2 = pDepthMarketData->AskVolume2;
	///申买价三
	tick_info.BidPrice3 = pDepthMarketData->BidPrice3;
	///申买量三
	tick_info.BidVolume3 = pDepthMarketData->BidVolume3;
	///申卖价三
	tick_info.AskPrice3 = pDepthMarketData->AskPrice3;
	///申卖量三
	tick_info.AskVolume3 = pDepthMarketData->AskVolume3;
	///申买价四
	tick_info.BidPrice4 = pDepthMarketData->BidPrice4;
	///申买量四
	tick_info.BidVolume4 = pDepthMarketData->BidVolume4;
	///申卖价四
	tick_info.AskPrice4 = pDepthMarketData->AskPrice4;
	///申卖量四
	tick_info.AskVolume4 = pDepthMarketData->AskVolume4;
	///申买价五
	tick_info.BidPrice5 = pDepthMarketData->BidPrice5;
	///申买量五
	tick_info.BidVolume5 = pDepthMarketData->BidVolume5;
	///申卖价五
	tick_info.AskPrice5 = pDepthMarketData->AskPrice5;
	///申卖量五
	tick_info.AskVolume5 = pDepthMarketData->AskVolume5;
	///当日均价
	tick_info.AveragePrice = pDepthMarketData->AveragePrice;
	///业务日期
	strcpy(tick_info.ActionDay, pDepthMarketData->ActionDay);
	this->mdevent->AddTick(tick_info);

	//std::cout << "OnRtnDepthMarketData,"
	//	<< pDepthMarketData->InstrumentID << ","
	//	<< pDepthMarketData->ExchangeID << ","
	//	<< pDepthMarketData->LastPrice << ","
	//	<< pDepthMarketData->OpenPrice << ","
	//	<< pDepthMarketData->HighestPrice << ","
	//	<< pDepthMarketData->LowestPrice << ","
	//	<< pDepthMarketData->Volume << ","
	//	<< pDepthMarketData->Turnover << ","
	//	<< pDepthMarketData->OpenInterest << ","
	//	<< pDepthMarketData->ClosePrice << ","
	//	<< pDepthMarketData->ActionDay << ","
	//	<< pDepthMarketData->UpdateTime << ","
	//	<< pDepthMarketData->UpdateMillisec << ","
	//	<< pDepthMarketData->AskPrice1 << ","
	//	<< pDepthMarketData->AskVolume1 << ","
	//	<< pDepthMarketData->BidPrice1 << ","
	//	<< pDepthMarketData->BidVolume1 << ","
	//	<< std::endl;
}