#pragma once

#ifndef _CTPTD_H_
#define _CTPTD_H_

#include "ThostFtdcTraderApi.h"

#include "DataStruct.h"

using namespace std;

class CtpTD : public CThostFtdcTraderSpi
{
public:
	CtpTD(CThostFtdcTraderApi* api, CThostFtdcReqUserLoginField* pUserLogin);
	~CtpTD();


	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse) {};

	///�ͻ�����֤��Ӧ
	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField* pRspAuthenticateField, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);


	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ������Ϣȷ����Ӧ
	virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);


	///�����ѯͶ���߽�������Ӧ
	void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField* pSettlementInfo, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);


	///Ͷ���߽�����ȷ����Ӧ
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///�ǳ�������Ӧ
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///�û��������������Ӧ
	virtual void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField* pUserPasswordUpdate, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///�ʽ��˻��������������Ӧ
	virtual void OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField* pTradingAccountPasswordUpdate, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};


	///�����ѯ��Լ��Ӧ
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///��֤��Ϣ
	void InLoginInfo(LoginInfo* login_info);

	std::vector<CThostFtdcInstrumentField> getInstruments()
	{
		return instruments;
	}

	

	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����֪ͨ
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);
	///�ɽ�֪ͨ
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);
	///�ֲ�֪ͨ
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///�ʽ�֪ͨ
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);


	///REQ
	///����¼������
	CThostFtdcOrderField ReqOrderInsert(CThostFtdcInputOrderField);
	///����
	int ReqOrderAction(CThostFtdcInputOrderActionField pInputOrderAction);
	///��ѯ�ֲ�
	CThostFtdcInvestorPositionField ReqQryInvestorPosition(CThostFtdcQryInvestorPositionField pQryInvestorPosition);
	CThostFtdcTradingAccountField ReqQryTradingAccount();

	vector<CThostFtdcTradeField>* OnRtnTradeVtr();

public:
	CThostFtdcTraderApi* api = nullptr;

private:
	CThostFtdcReqUserLoginField* userLogin = nullptr;
	CThostFtdcRspUserLoginField* userLogin_RSP = nullptr;
	int requestID;
	std::string tradingDate;
	std::vector<CThostFtdcInstrumentField> instruments;
	CThostFtdcOrderField *rtn_order;
	CThostFtdcInvestorPositionField* rtn_position;
	CThostFtdcTradingAccountField* rtn_acc;

	LoginInfo* login_info;

	vector<CThostFtdcTradeField> __onRtnTrade_vtr;
};

#endif 