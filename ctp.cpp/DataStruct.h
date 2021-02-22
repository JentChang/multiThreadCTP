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
	///@param FRONT_ADDR_MD行情地址
	///@param FRONT_ADDR_TD交易地址
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@param authenticate客户端认证请求
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
//	///交易日
//	char	TradingDay[9];
//	///合约代码
//	char	InstrumentID[31];
//	///交易所代码
//	char	ExchangeID[9];
//	///合约在交易所的代码
//	char	ExchangeInstID[31];
//	///最新价
//	double	LastPrice;
//	///上次结算价
//	double	PreSettlementPrice;
//	///昨收盘
//	double	PreClosePrice;
//	///昨持仓量
//	double	PreOpenInterest;
//	///今开盘
//	double	OpenPrice;
//	///最高价
//	double	HighestPrice;
//	///最低价
//	double	LowestPrice;
//	///数量
//	int	Volume;
//	///成交金额
//	double	Turnover;
//	///持仓量
//	double	OpenInterest;
//	///今收盘
//	double	ClosePrice;
//	///本次结算价
//	double	SettlementPrice;
//	///涨停板价
//	double	UpperLimitPrice;
//	///跌停板价
//	double	LowerLimitPrice;
//	///昨虚实度
//	double	PreDelta;
//	///今虚实度
//	double	CurrDelta;
//	///最后修改时间
//	char	UpdateTime[9];
//	///最后修改毫秒
//	int	UpdateMillisec;
//	///申买价一
//	double	BidPrice1;
//	///申买量一
//	int	BidVolume1;
//	///申卖价一
//	double	AskPrice1;
//	///申卖量一
//	int	AskVolume1;
//	///申买价二
//	double	BidPrice2;
//	///申买量二
//	int	BidVolume2;
//	///申卖价二
//	double	AskPrice2;
//	///申卖量二
//	int	AskVolume2;
//	///申买价三
//	double	BidPrice3;
//	///申买量三
//	int	BidVolume3;
//	///申卖价三
//	double	AskPrice3;
//	///申卖量三
//	int	AskVolume3;
//	///申买价四
//	double	BidPrice4;
//	///申买量四
//	int	BidVolume4;
//	///申卖价四
//	double	AskPrice4;
//	///申卖量四
//	int	AskVolume4;
//	///申买价五
//	double	BidPrice5;
//	///申买量五
//	int	BidVolume5;
//	///申卖价五
//	double	AskPrice5;
//	///申卖量五
//	int	AskVolume5;
//	///当日均价
//	double	AveragePrice;
//	///业务日期
//	char	ActionDay[9];
//};


struct TickInfomation
{
	bool EMPTY = true;
	char datetime[24];

	///交易日
	TThostFtdcDateType	TradingDay;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///合约在交易所的代码
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///最新价
	TThostFtdcPriceType	LastPrice;
	///上次结算价
	TThostFtdcPriceType	PreSettlementPrice;
	///昨收盘
	TThostFtdcPriceType	PreClosePrice;
	///昨持仓量
	TThostFtdcLargeVolumeType	PreOpenInterest;
	///今开盘
	TThostFtdcPriceType	OpenPrice;
	///最高价
	TThostFtdcPriceType	HighestPrice;
	///最低价
	TThostFtdcPriceType	LowestPrice;
	///数量
	TThostFtdcVolumeType	Volume;
	///成交金额
	TThostFtdcMoneyType	Turnover;
	///持仓量
	TThostFtdcLargeVolumeType	OpenInterest;
	///今收盘
	TThostFtdcPriceType	ClosePrice;
	///本次结算价
	TThostFtdcPriceType	SettlementPrice;
	///涨停板价
	TThostFtdcPriceType	UpperLimitPrice;
	///跌停板价
	TThostFtdcPriceType	LowerLimitPrice;
	///昨虚实度
	TThostFtdcRatioType	PreDelta;
	///今虚实度
	TThostFtdcRatioType	CurrDelta;
	///最后修改时间
	TThostFtdcTimeType	UpdateTime;
	///最后修改毫秒
	TThostFtdcMillisecType	UpdateMillisec;
	///申买价一
	TThostFtdcPriceType	BidPrice1;
	///申买量一
	TThostFtdcVolumeType	BidVolume1;
	///申卖价一
	TThostFtdcPriceType	AskPrice1;
	///申卖量一
	TThostFtdcVolumeType	AskVolume1;
	///申买价二
	TThostFtdcPriceType	BidPrice2;
	///申买量二
	TThostFtdcVolumeType	BidVolume2;
	///申卖价二
	TThostFtdcPriceType	AskPrice2;
	///申卖量二
	TThostFtdcVolumeType	AskVolume2;
	///申买价三
	TThostFtdcPriceType	BidPrice3;
	///申买量三
	TThostFtdcVolumeType	BidVolume3;
	///申卖价三
	TThostFtdcPriceType	AskPrice3;
	///申卖量三
	TThostFtdcVolumeType	AskVolume3;
	///申买价四
	TThostFtdcPriceType	BidPrice4;
	///申买量四
	TThostFtdcVolumeType	BidVolume4;
	///申卖价四
	TThostFtdcPriceType	AskPrice4;
	///申卖量四
	TThostFtdcVolumeType	AskVolume4;
	///申买价五
	TThostFtdcPriceType	BidPrice5;
	///申买量五
	TThostFtdcVolumeType	BidVolume5;
	///申卖价五
	TThostFtdcPriceType	AskPrice5;
	///申卖量五
	TThostFtdcVolumeType	AskVolume5;
	///当日均价
	TThostFtdcPriceType	AveragePrice;
	///业务日期
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

	///昨收盘
	TThostFtdcPriceType	PreClosePrice;
	///昨持仓量
	TThostFtdcLargeVolumeType	PreOpenInterest;
};


///字符串切分
vector<string> Split(const string& src, string separate_character);


#endif