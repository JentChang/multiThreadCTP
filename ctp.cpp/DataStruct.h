#pragma once
#ifndef _DataStruct_H_
#define _DataStruct_H_

#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcTraderApi.h"
//#include <vector>
#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <mutex>
#include <math.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <cstring>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <queue>
#include <map>



using namespace std;

struct LoginInfo
{
	///@param FRONT_ADDR_MD�����ַ
	///@param FRONT_ADDR_TD���׵�ַ
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@param authenticate�ͻ�����֤����
	char BrokerID[11];
	char UserID[16];
	char Password[41];
	
	char FRONT_ADDR_MD[100];
	char FRONT_ADDR_TD[100];

	char *ppInstrumentID[100];
	int nCount;

	char AuthCode[17];
	char AppID[33]; 
};


//struct TickInfomation
//{
//	bool EMPTY = true;
//	string datetime;
//
//	///������
//	char	TradingDay[9];
//	///��Լ����
//	char	InstrumentID[31];
//	///����������
//	char	ExchangeID[9];
//	///��Լ�ڽ������Ĵ���
//	char	ExchangeInstID[31];
//	///���¼�
//	double	LastPrice;
//	///�ϴν����
//	double	PreSettlementPrice;
//	///������
//	double	PreClosePrice;
//	///��ֲ���
//	double	PreOpenInterest;
//	///����
//	double	OpenPrice;
//	///��߼�
//	double	HighestPrice;
//	///��ͼ�
//	double	LowestPrice;
//	///����
//	int	Volume;
//	///�ɽ����
//	double	Turnover;
//	///�ֲ���
//	double	OpenInterest;
//	///������
//	double	ClosePrice;
//	///���ν����
//	double	SettlementPrice;
//	///��ͣ���
//	double	UpperLimitPrice;
//	///��ͣ���
//	double	LowerLimitPrice;
//	///����ʵ��
//	double	PreDelta;
//	///����ʵ��
//	double	CurrDelta;
//	///����޸�ʱ��
//	char	UpdateTime[9];
//	///����޸ĺ���
//	int	UpdateMillisec;
//	///�����һ
//	double	BidPrice1;
//	///������һ
//	int	BidVolume1;
//	///������һ
//	double	AskPrice1;
//	///������һ
//	int	AskVolume1;
//	///����۶�
//	double	BidPrice2;
//	///��������
//	int	BidVolume2;
//	///�����۶�
//	double	AskPrice2;
//	///��������
//	int	AskVolume2;
//	///�������
//	double	BidPrice3;
//	///��������
//	int	BidVolume3;
//	///��������
//	double	AskPrice3;
//	///��������
//	int	AskVolume3;
//	///�������
//	double	BidPrice4;
//	///��������
//	int	BidVolume4;
//	///��������
//	double	AskPrice4;
//	///��������
//	int	AskVolume4;
//	///�������
//	double	BidPrice5;
//	///��������
//	int	BidVolume5;
//	///��������
//	double	AskPrice5;
//	///��������
//	int	AskVolume5;
//	///���վ���
//	double	AveragePrice;
//	///ҵ������
//	char	ActionDay[9];
//};


struct TickInfomation
{
	bool EMPTY = true;
	char datetime[24];

	///������
	TThostFtdcDateType	TradingDay;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///���¼�
	TThostFtdcPriceType	LastPrice;
	///�ϴν����
	TThostFtdcPriceType	PreSettlementPrice;
	///������
	TThostFtdcPriceType	PreClosePrice;
	///��ֲ���
	TThostFtdcLargeVolumeType	PreOpenInterest;
	///����
	TThostFtdcPriceType	OpenPrice;
	///��߼�
	TThostFtdcPriceType	HighestPrice;
	///��ͼ�
	TThostFtdcPriceType	LowestPrice;
	///����
	TThostFtdcVolumeType	Volume;
	///�ɽ����
	TThostFtdcMoneyType	Turnover;
	///�ֲ���
	TThostFtdcLargeVolumeType	OpenInterest;
	///������
	TThostFtdcPriceType	ClosePrice;
	///���ν����
	TThostFtdcPriceType	SettlementPrice;
	///��ͣ���
	TThostFtdcPriceType	UpperLimitPrice;
	///��ͣ���
	TThostFtdcPriceType	LowerLimitPrice;
	///����ʵ��
	TThostFtdcRatioType	PreDelta;
	///����ʵ��
	TThostFtdcRatioType	CurrDelta;
	///����޸�ʱ��
	TThostFtdcTimeType	UpdateTime;
	///����޸ĺ���
	TThostFtdcMillisecType	UpdateMillisec;
	///�����һ
	TThostFtdcPriceType	BidPrice1;
	///������һ
	TThostFtdcVolumeType	BidVolume1;
	///������һ
	TThostFtdcPriceType	AskPrice1;
	///������һ
	TThostFtdcVolumeType	AskVolume1;
	///����۶�
	TThostFtdcPriceType	BidPrice2;
	///��������
	TThostFtdcVolumeType	BidVolume2;
	///�����۶�
	TThostFtdcPriceType	AskPrice2;
	///��������
	TThostFtdcVolumeType	AskVolume2;
	///�������
	TThostFtdcPriceType	BidPrice3;
	///��������
	TThostFtdcVolumeType	BidVolume3;
	///��������
	TThostFtdcPriceType	AskPrice3;
	///��������
	TThostFtdcVolumeType	AskVolume3;
	///�������
	TThostFtdcPriceType	BidPrice4;
	///��������
	TThostFtdcVolumeType	BidVolume4;
	///��������
	TThostFtdcPriceType	AskPrice4;
	///��������
	TThostFtdcVolumeType	AskVolume4;
	///�������
	TThostFtdcPriceType	BidPrice5;
	///��������
	TThostFtdcVolumeType	BidVolume5;
	///��������
	TThostFtdcPriceType	AskPrice5;
	///��������
	TThostFtdcVolumeType	AskVolume5;
	///���վ���
	TThostFtdcPriceType	AveragePrice;
	///ҵ������
	TThostFtdcDateType	ActionDay;
};


struct BarInfomation
{
	bool EMPTY = true;
	TThostFtdcInstrumentIDType InstrumentID;
	TThostFtdcDateType	TradingDay;
	TThostFtdcTimeType	UpdateTime;

	TThostFtdcPriceType Close;
	TThostFtdcPriceType High;
	TThostFtdcPriceType Low;
	TThostFtdcPriceType Open;

	TThostFtdcVolumeType Volume;
	TThostFtdcMoneyType	Turnover;

	///������
	TThostFtdcPriceType	PreClosePrice;
	///��ֲ���
	TThostFtdcLargeVolumeType	PreOpenInterest;
};


///�ַ����з�
vector<string> Split(const string& src, string separate_character);


#endif