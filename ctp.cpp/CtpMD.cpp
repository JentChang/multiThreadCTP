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


///����Ӧ��
void CtpMD::OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "OnRspError:"
		<< pRspInfo->ErrorID
		<< pRspInfo->ErrorMsg
		<< std::endl;
}

///��������Ӧ��
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


///�������֪ͨ
void CtpMD::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
	TickInfomation tick_info;
	tick_info.EMPTY = false;
	///������
	strcpy(tick_info.TradingDay, pDepthMarketData->TradingDay);
	///��Լ����
	strcpy(tick_info.InstrumentID, pDepthMarketData->InstrumentID);
	///����������
	strcpy(tick_info.ExchangeID, pDepthMarketData->ExchangeID);
	///��Լ�ڽ������Ĵ���
	strcpy(tick_info.ExchangeInstID, pDepthMarketData->ExchangeInstID);
	///���¼�
	tick_info.LastPrice = pDepthMarketData->LastPrice;
	///�ϴν����
	tick_info.PreSettlementPrice = pDepthMarketData->PreSettlementPrice;
	///������
	tick_info.PreClosePrice = pDepthMarketData->PreClosePrice;
	///��ֲ���
	tick_info.PreOpenInterest = pDepthMarketData->PreOpenInterest;
	///����
	tick_info.OpenPrice = pDepthMarketData->OpenPrice;
	///��߼�
	tick_info.HighestPrice = pDepthMarketData->HighestPrice;
	///��ͼ�
	tick_info.LowestPrice = pDepthMarketData->LowestPrice;
	///����
	tick_info.Volume = pDepthMarketData->Volume;
	///�ɽ����
	tick_info.Turnover = pDepthMarketData->Turnover;
	///�ֲ���
	tick_info.OpenInterest = pDepthMarketData->OpenInterest;
	///������
	tick_info.ClosePrice = pDepthMarketData->ClosePrice;
	///���ν����
	tick_info.SettlementPrice = pDepthMarketData->SettlementPrice;
	///��ͣ���
	tick_info.UpperLimitPrice = pDepthMarketData->UpperLimitPrice;
	///��ͣ���
	tick_info.LowerLimitPrice = pDepthMarketData->LowerLimitPrice;
	///����ʵ��
	tick_info.PreDelta = pDepthMarketData->PreDelta;
	///����ʵ��
	tick_info.CurrDelta = pDepthMarketData->CurrDelta;
	///����޸�ʱ��
	strcpy(tick_info.UpdateTime, pDepthMarketData->UpdateTime);
	///����޸ĺ���
	tick_info.UpdateMillisec = pDepthMarketData->UpdateMillisec;
	///�����һ
	tick_info.BidPrice1 = pDepthMarketData->BidPrice1;
	///������һ
	tick_info.BidVolume1 = pDepthMarketData->BidVolume1;
	///������һ
	tick_info.AskPrice1 = pDepthMarketData->AskPrice1;
	///������һ
	tick_info.AskVolume1 = pDepthMarketData->AskVolume1;
	///����۶�
	tick_info.BidPrice2 = pDepthMarketData->BidPrice2;
	///��������
	tick_info.BidVolume2 = pDepthMarketData->BidVolume2;
	///�����۶�
	tick_info.AskPrice2 = pDepthMarketData->AskPrice2;
	///��������
	tick_info.AskVolume2 = pDepthMarketData->AskVolume2;
	///�������
	tick_info.BidPrice3 = pDepthMarketData->BidPrice3;
	///��������
	tick_info.BidVolume3 = pDepthMarketData->BidVolume3;
	///��������
	tick_info.AskPrice3 = pDepthMarketData->AskPrice3;
	///��������
	tick_info.AskVolume3 = pDepthMarketData->AskVolume3;
	///�������
	tick_info.BidPrice4 = pDepthMarketData->BidPrice4;
	///��������
	tick_info.BidVolume4 = pDepthMarketData->BidVolume4;
	///��������
	tick_info.AskPrice4 = pDepthMarketData->AskPrice4;
	///��������
	tick_info.AskVolume4 = pDepthMarketData->AskVolume4;
	///�������
	tick_info.BidPrice5 = pDepthMarketData->BidPrice5;
	///��������
	tick_info.BidVolume5 = pDepthMarketData->BidVolume5;
	///��������
	tick_info.AskPrice5 = pDepthMarketData->AskPrice5;
	///��������
	tick_info.AskVolume5 = pDepthMarketData->AskVolume5;
	///���վ���
	tick_info.AveragePrice = pDepthMarketData->AveragePrice;
	///ҵ������
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