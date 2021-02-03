#include "CtpTD.h"
#include <Windows.h>



CtpTD::CtpTD(CThostFtdcTraderApi* api, CThostFtdcReqUserLoginField* pUserLogin) :
	api(api), userLogin(pUserLogin)
{
	this->requestID = 1;
	this->__onRtnTrade_vtr.clear();
	this->__onRtnTrade_vtr.resize(500);
}


CtpTD::~CtpTD()
{
	this->api = nullptr;
	this->userLogin = nullptr;
	this->userLogin_RSP = nullptr;
}


void CtpTD::OnFrontConnected()
{
	std::cout << "OnFrontConnected..." << std::endl;
	if (userLogin)
	{
		//api->ReqUserLogin(userLogin, requestID++);
		CThostFtdcReqAuthenticateField auth_info = { 0 };
		strncpy(auth_info.BrokerID, userLogin->BrokerID, sizeof(TThostFtdcBrokerIDType));
		strncpy(auth_info.UserID, userLogin->UserID, sizeof(TThostFtdcUserIDType));

		strcpy(auth_info.AuthCode, this->login_info->AuthCode);
		strcpy(auth_info.AppID, this->login_info->AppID);

		int AuthRet = api->ReqAuthenticate(&auth_info, requestID++);
		std::cout << "ReqAuthenticate response:" << AuthRet << std::endl;

	}
}

void CtpTD::OnFrontDisconnected(int nReason)
{
	std::cout << "OnFrontDisconnected, nReason:" << nReason << std::endl;
}

void CtpTD::OnRspAuthenticate(CThostFtdcRspAuthenticateField * pRspAuthenticateField, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		std::cout << "OnRspAuthenticate ok" << std::endl;
		api->ReqUserLogin(userLogin, requestID++);
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "OnRspAuthenticate error:"
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
}

///��¼������Ӧ
void CtpTD::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		std::cout << "OnRspUserLogin ok" << std::endl;
		this->tradingDate = api->GetTradingDay();

		this->userLogin_RSP = pRspUserLogin;

		std::cout << "TradingDay:" << tradingDate.c_str() << std::endl;
		cout << "��¼�ɹ�. " << endl
			<< "ǰ�ñ��:" << pRspUserLogin->FrontID << endl
			<< "�Ự���" << pRspUserLogin->SessionID << endl
			<< "��󱨵�����:" << pRspUserLogin->MaxOrderRef << endl
			<< "������ʱ�䣺" << pRspUserLogin->SHFETime << endl
			<< "������ʱ�䣺" << pRspUserLogin->DCETime << endl
			<< "֣����ʱ�䣺" << pRspUserLogin->CZCETime << endl
			<< "�н���ʱ�䣺" << pRspUserLogin->FFEXTime << endl
			<< "��Դ����ʱ�䣺" << pRspUserLogin->INETime << endl;
		
		//���ڻ��̵�һ�η��ͽ���ָ��ǰ����Ҫ�Ȳ�ѯͶ���߽������� Ͷ����ȷ���Ժ���ܽ��ס�
		//��ѯ�Ƿ��Ѿ�����ȷ��
		CThostFtdcSettlementInfoConfirmField pSettlementInfoConfirm;
		memset(&pSettlementInfoConfirm, 0, sizeof(CThostFtdcSettlementInfoConfirmField));
		strncpy(pSettlementInfoConfirm.BrokerID, pRspUserLogin->BrokerID, sizeof(TThostFtdcBrokerIDType));
		strncpy(pSettlementInfoConfirm.InvestorID, pRspUserLogin->UserID, sizeof(TThostFtdcInvestorIDType));
		api->ReqSettlementInfoConfirm(&pSettlementInfoConfirm, requestID++);
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "OnRspUserLogin error:"
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
}


///�����ѯ������Ϣȷ����Ӧ
void CtpTD::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		if (nullptr == pSettlementInfoConfirm)
		{
			std::cout << "OnRspQrySettlementInfoConfirm:"
				<< pSettlementInfoConfirm->ConfirmDate << ","
				<< pSettlementInfoConfirm->ConfirmTime << ","
				<< bIsLast
				<< std::endl;


			string lastConfirmDate = pSettlementInfoConfirm->ConfirmDate;
			cout << lastConfirmDate.c_str() << endl;
			cout << pSettlementInfoConfirm->ConfirmTime << endl;

			if (lastConfirmDate != tradingDate) {
				//���컹ûȷ�Ϲ�, ��һ�η��ͽ���ָ��ǰ����ѯͶ���߽�����
				std::this_thread::sleep_for(std::chrono::seconds(1));

				CThostFtdcQrySettlementInfoField field;
				memset(&field, 0, sizeof(CThostFtdcQrySettlementInfoField));
				strncpy(field.BrokerID, pSettlementInfoConfirm->BrokerID, sizeof(TThostFtdcBrokerIDType));
				strncpy(field.InvestorID, pSettlementInfoConfirm->InvestorID, sizeof(TThostFtdcInvestorIDType));
				strncpy(field.TradingDay, pSettlementInfoConfirm->ConfirmDate, sizeof(TThostFtdcDateType));

				api->ReqQrySettlementInfo(&field, requestID++);
			}
			else {
				//�����Ѿ�ȷ��
				CThostFtdcQryTradingAccountField account;
				memset(&account, 0, sizeof(CThostFtdcQryTradingAccountField));
				strncpy(account.BrokerID, pSettlementInfoConfirm->BrokerID, sizeof(TThostFtdcBrokerIDType));
				strncpy(account.InvestorID, pSettlementInfoConfirm->InvestorID, sizeof(TThostFtdcInvestorIDType));
				api->ReqQryTradingAccount(&account, requestID++);
			}

		}
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "OnRspQrySettlementInfoConfirm error:"
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
}

///�����ѯͶ���߽�������Ӧ
void CtpTD::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	cout << "�����ѯͶ���߽�������Ӧ, ,  ID: " << nRequestID << endl;
	cout << pSettlementInfo->Content << endl;
	if (pRspInfo == nullptr || pRspInfo->ErrorID == 0) {
		if (bIsLast == true) {
			//ȷ��Ͷ���߽�����
			CThostFtdcSettlementInfoConfirmField field;
			memset(&field, 0, sizeof(CThostFtdcQryTradingAccountField));
			strncpy(field.BrokerID, pSettlementInfo->BrokerID, sizeof(TThostFtdcBrokerIDType));
			strncpy(field.InvestorID, pSettlementInfo->InvestorID, sizeof(TThostFtdcInvestorIDType));
			int result = api->ReqSettlementInfoConfirm(&field, requestID++);
			cout << "result:" << result << endl;
		}
	}
	else {
		cout << "����Ӧ��:" << pRspInfo->ErrorID << pRspInfo->ErrorMsg << " ����IDΪ" << requestID << endl;
	}
}


void CtpTD::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		if (pSettlementInfoConfirm != nullptr) {
			cout << "���͹�˾����:" << pSettlementInfoConfirm->BrokerID << endl
				<< "�û��˺�:" << pSettlementInfoConfirm->InvestorID << endl
				<< "ȷ�����ڣ�" << pSettlementInfoConfirm->ConfirmDate << endl
				<< "ȷ��ʱ�䣺" << pSettlementInfoConfirm->ConfirmTime << endl;

			//��ѯ���п��ú�Լ��Ϣ
			CThostFtdcQryInstrumentField myreq = CThostFtdcQryInstrumentField();
			memset(&myreq, 0, sizeof(CThostFtdcQryInstrumentField));
			api->ReqQryInstrument(&myreq, requestID++);
		}
		else {
			cout << "Ͷ���߽�����ȷ����ӦΪ��, pSettlementInfoConfirm:" << pSettlementInfoConfirm << endl;
		}
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "OnRspUserLogin error:"
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
}



///�����ѯ��Լ��Ӧ
void CtpTD::OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		if (bIsLast)
		{
			//do nothing
		}
		else {
			CThostFtdcInstrumentField instrument;
			memcpy(&instrument, pInstrument, sizeof(CThostFtdcInstrumentField));
			instruments.push_back(instrument);
		}
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "OnRspQryInstrument error:"
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
}

void CtpTD::InLoginInfo(LoginInfo* login_info)
{
	this->login_info = login_info;
}

int CtpTD::ReqOrderInsert(CThostFtdcInputOrderField order)
{
	///���͹�˾����
	strcpy(order.BrokerID, this->login_info->BrokerID);
	///Ͷ���ߴ���
	strcpy(order.InvestorID, this->login_info->UserID);
	strcpy(order.UserID, this->login_info->UserID);
	///��������
	sprintf(order.OrderRef, "%d", this->requestID++);
	///�û�����
//	TThostFtdcUserIDType	UserID;
	///�����۸�����: �޼�
	//req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;

	///���Ͷ���ױ���־
	order.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;

	///GTD����
//	TThostFtdcDateType	GTDDate;

	//req.VolumeCondition = THOST_FTDC_VC_CV;

	///ֹ���
//	TThostFtdcPriceType	StopPrice;
	///ǿƽԭ��: ��ǿƽ
	order.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///�Զ������־: ��
	order.IsAutoSuspend = 0;
	///ҵ��Ԫ
//	TThostFtdcBusinessUnitType	BusinessUnit;
	///������
//	TThostFtdcRequestIDType	RequestID;
	///�û�ǿ����־: ��
	order.UserForceClose = 0;

	int iResult = this->api->ReqOrderInsert(&order, ++requestID);
	cerr << "--->>> ����¼������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
	if (iResult == 0)
	{
		return requestID;
	}
	else
	{
		return -1;
	}
	
}

void CtpTD::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspOrderInsert" << endl;
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		if (bIsLast)
		{
			//do nothing
		}
		else {

		}
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "OnRspOrderInsert error:"
			<< pRspInfo->ErrorID << ", "
			<< pRspInfo->ErrorMsg
			<< std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
}

///����֪ͨ
void CtpTD::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	/*
	///���͹�˾����
	TThostFtdcBrokerIDType BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType InstrumentID;
	///��������
	TThostFtdcOrderRefType OrderRef;
	///�û�����
	TThostFtdcUserIDType UserID;
	///�����۸�����
	TThostFtdcOrderPriceTypeType OrderPriceType;
	///��������
	TThostFtdcDirectionType Direction;
	///��Ͽ�ƽ��־
	TThostFtdcCombOffsetFlagType CombOffsetFlag;
	///���Ͷ���ױ���־
	TThostFtdcCombHedgeFlagType CombHedgeFlag;
	///�۸�
	TThostFtdcPriceType LimitPrice;
	///����
	TThostFtdcVolumeType VolumeTotalOriginal;
	///��Ч������
	TThostFtdcTimeConditionType TimeCondition;
	///GTD����
	TThostFtdcDateType GTDDate;
	///�ɽ�������
	TThostFtdcVolumeConditionType VolumeCondition;
	///��С�ɽ���
	TThostFtdcVolumeType MinVolume;
	///��������
	TThostFtdcContingentConditionType ContingentCondition;
	///ֹ���
	TThostFtdcPriceType StopPrice;
	///ǿƽԭ��
	TThostFtdcForceCloseReasonType ForceCloseReason;
	///�Զ������־
	TThostFtdcBoolType IsAutoSuspend;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType BusinessUnit;
	///������
	TThostFtdcRequestIDType RequestID;
	///���ر������
	TThostFtdcOrderLocalIDType OrderLocalID;
	///����������
	TThostFtdcExchangeIDType ExchangeID;
	///��Ա����
	TThostFtdcParticipantIDType ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType ClientID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType ExchangeInstID;
	///����������Ա����
	TThostFtdcTraderIDType TraderID;
	///��װ���
	TThostFtdcInstallIDType InstallID;
	///�����ύ״̬
	TThostFtdcOrderSubmitStatusType OrderSubmitStatus;
	///������ʾ���
	TThostFtdcSequenceNoType NotifySequence;
	///������
	TThostFtdcDateType TradingDay;
	///������
	TThostFtdcSettlementIDType SettlementID;
	///�������
	TThostFtdcOrderSysIDType OrderSysID;
	///������Դ
	TThostFtdcOrderSourceType OrderSource;
	///����״̬
	TThostFtdcOrderStatusType OrderStatus;
	///��������
	TThostFtdcOrderTypeType OrderType;
	///��ɽ�����
	TThostFtdcVolumeType VolumeTraded;
	///ʣ������
	TThostFtdcVolumeType VolumeTotal;
	///��������
	TThostFtdcDateType InsertDate;
	///ί��ʱ��
	TThostFtdcTimeType InsertTime;
	///����ʱ��
	TThostFtdcTimeType ActiveTime;
	///����ʱ��
	TThostFtdcTimeType SuspendTime;
	///����޸�ʱ��
	TThostFtdcTimeType UpdateTime;
	///����ʱ��
	TThostFtdcTimeType CancelTime;
	///����޸Ľ���������Ա����
	TThostFtdcTraderIDType ActiveTraderID;
	///�����Ա���
	TThostFtdcParticipantIDType ClearingPartID;
	///���
	TThostFtdcSequenceNoType SequenceNo;
	///ǰ�ñ��
	TThostFtdcFrontIDType FrontID;
	///�Ự���
	TThostFtdcSessionIDType SessionID;
	///�û��˲�Ʒ��Ϣ
	TThostFtdcProductInfoType UserProductInfo;
	///״̬��Ϣ
	TThostFtdcErrorMsgType StatusMsg;
	///�û�ǿ����־
	TThostFtdcBoolType UserForceClose;
	///�����û�����
	TThostFtdcUserIDType ActiveUserID;
	///���͹�˾�������
	TThostFtdcSequenceNoType BrokerOrderSeq;
	///��ر���
	TThostFtdcOrderSysIDType RelativeOrderSysID;
	///֣�����ɽ�����
	TThostFtdcVolumeType ZCETotalTradedVolume;
	///��������־
	TThostFtdcBoolType IsSwapOrder;
	///Ӫҵ�����
	TThostFtdcBranchIDType BranchID;
	///Ͷ�ʵ�Ԫ����
	TThostFtdcInvestUnitIDType InvestUnitID;
	///�ʽ��˺�
	TThostFtdcAccountIDType AccountID;
	///���ִ���
	TThostFtdcCurrencyIDType CurrencyID;
	///IP��ַ
	TThostFtdcIPAddressType IPAddress;
	///Mac��ַ
	TThostFtdcMacAddressType MacAddress;
*/
}

void CtpTD::OnRtnTrade(CThostFtdcTradeField * pTrade)
{
	this->__onRtnTrade_vtr.push_back(*pTrade);
	this->__onRtnTrade_vtr.erase(this->__onRtnTrade_vtr.begin());
	//FrontID + SessionID + OrderRef
		/////���͹�˾����
		//TThostFtdcBrokerIDType	BrokerID;
		/////Ͷ���ߴ���
		//TThostFtdcInvestorIDType	InvestorID;
		/////��Լ����
		//TThostFtdcInstrumentIDType	InstrumentID;
	/////��������
	//TThostFtdcOrderRefType	OrderRef;
		/////�û�����
		//TThostFtdcUserIDType	UserID;
		/////����������
		//TThostFtdcExchangeIDType	ExchangeID;
		/////�ɽ����
		//TThostFtdcTradeIDType	TradeID;
		/////��������
		//TThostFtdcDirectionType	Direction;
	/////�������
	//TThostFtdcOrderSysIDType	OrderSysID;
		/////��Ա����
		//TThostFtdcParticipantIDType	ParticipantID;
		/////�ͻ�����
		//TThostFtdcClientIDType	ClientID;
		/////���׽�ɫ
		//TThostFtdcTradingRoleType	TradingRole;
		/////��Լ�ڽ������Ĵ���
		//TThostFtdcExchangeInstIDType	ExchangeInstID;
		/////��ƽ��־
		//TThostFtdcOffsetFlagType	OffsetFlag;
		/////Ͷ���ױ���־
		//TThostFtdcHedgeFlagType	HedgeFlag;
		/////�۸�
		//TThostFtdcPriceType	Price;
		/////����
		//TThostFtdcVolumeType	Volume;
		/////�ɽ�ʱ��
		//TThostFtdcDateType	TradeDate;
		/////�ɽ�ʱ��
		//TThostFtdcTimeType	TradeTime;
		/////�ɽ�����
		//TThostFtdcTradeTypeType	TradeType;
		/////�ɽ�����Դ
		//TThostFtdcPriceSourceType	PriceSource;
		/////����������Ա����
		//TThostFtdcTraderIDType	TraderID;
		/////���ر������
		//TThostFtdcOrderLocalIDType	OrderLocalID;
		/////�����Ա���
		//TThostFtdcParticipantIDType	ClearingPartID;
		/////ҵ��Ԫ
		//TThostFtdcBusinessUnitType	BusinessUnit;
		/////���
		//TThostFtdcSequenceNoType	SequenceNo;
		/////������
		//TThostFtdcDateType	TradingDay;
		/////������
		//TThostFtdcSettlementIDType	SettlementID;
		/////���͹�˾�������
		//TThostFtdcSequenceNoType	BrokerOrderSeq;
		/////�ɽ���Դ
		//TThostFtdcTradeSourceType	TradeSource;
		/////Ͷ�ʵ�Ԫ����
		//TThostFtdcInvestUnitIDType	InvestUnitID;

}

int CtpTD::ReqOrderAction(CThostFtdcInputOrderActionField pInputOrderAction)
{
	this->requestID++;
	///���͹�˾����
	strcpy(pInputOrderAction.BrokerID, this->login_info->BrokerID);
	///Ͷ���ߴ���
	strcpy(pInputOrderAction.InvestorID, this->login_info->UserID);
	strcpy(pInputOrderAction.UserID, this->login_info->UserID);
	///������������
	pInputOrderAction.OrderActionRef = this->requestID;
	///ǰ�ñ��
	pInputOrderAction.FrontID = this->userLogin_RSP->FrontID;
	///�Ự���
	pInputOrderAction.SessionID = this->userLogin_RSP->SessionID;


	return requestID;
}

vector<CThostFtdcTradeField>* CtpTD::OnRtnTradeVtr()
{
	return &this->__onRtnTrade_vtr;
}

