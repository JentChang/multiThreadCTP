#pragma once

#ifndef _CTPTD_H_
#define _CTPTD_H_


#include <iostream>
#include <vector>
#include <thread>

#include "ThostFtdcTraderApi.h"

#include "DataStruct.h"

using namespace std;

class CtpTD : public CThostFtdcTraderSpi
{
public:
	CtpTD(CThostFtdcTraderApi* api, CThostFtdcReqUserLoginField* pUserLogin);
	~CtpTD();


	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason);

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	virtual void OnHeartBeatWarning(int nTimeLapse) {};

	///客户端认证响应
	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField* pRspAuthenticateField, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);


	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///请求查询结算信息确认响应
	virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);


	///请求查询投资者结算结果响应
	void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField* pSettlementInfo, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);


	///投资者结算结果确认响应
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///登出请求响应
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///用户口令更新请求响应
	virtual void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField* pUserPasswordUpdate, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///资金账户口令更新请求响应
	virtual void OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField* pTradingAccountPasswordUpdate, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};


	///请求查询合约响应
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///验证信息
	void InLoginInfo(LoginInfo* login_info);

	std::vector<CThostFtdcInstrumentField> getInstruments()
	{
		return instruments;
	}

	///报单录入请求
	int ReqOrderInsert(CThostFtdcInputOrderField);

	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单通知
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

	///成交通知
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

	///撤单
	int ReqOrderAction(CThostFtdcInputOrderActionField pInputOrderAction);

	///成交回报
	vector<CThostFtdcTradeField>* OnRtnTradeVtr();

private:
	

private:
	CThostFtdcTraderApi* api = nullptr;

	CThostFtdcReqUserLoginField* userLogin = nullptr;
	CThostFtdcRspUserLoginField* userLogin_RSP = nullptr;
	int requestID;
	std::string tradingDate;
	std::vector<CThostFtdcInstrumentField> instruments;

	LoginInfo* login_info;

	vector<CThostFtdcTradeField> __onRtnTrade_vtr;
};

#endif 