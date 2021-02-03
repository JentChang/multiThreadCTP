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

///登录请求响应
void CtpTD::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		std::cout << "OnRspUserLogin ok" << std::endl;
		this->tradingDate = api->GetTradingDay();

		this->userLogin_RSP = pRspUserLogin;

		std::cout << "TradingDay:" << tradingDate.c_str() << std::endl;
		cout << "登录成功. " << endl
			<< "前置编号:" << pRspUserLogin->FrontID << endl
			<< "会话编号" << pRspUserLogin->SessionID << endl
			<< "最大报单引用:" << pRspUserLogin->MaxOrderRef << endl
			<< "上期所时间：" << pRspUserLogin->SHFETime << endl
			<< "大商所时间：" << pRspUserLogin->DCETime << endl
			<< "郑商所时间：" << pRspUserLogin->CZCETime << endl
			<< "中金所时间：" << pRspUserLogin->FFEXTime << endl
			<< "能源中心时间：" << pRspUserLogin->INETime << endl;
		
		//向期货商第一次发送交易指令前，需要先查询投资者结算结果， 投资者确认以后才能交易。
		//查询是否已经做了确认
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


///请求查询结算信息确认响应
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
				//今天还没确认过, 第一次发送交易指令前，查询投资者结算结果
				std::this_thread::sleep_for(std::chrono::seconds(1));

				CThostFtdcQrySettlementInfoField field;
				memset(&field, 0, sizeof(CThostFtdcQrySettlementInfoField));
				strncpy(field.BrokerID, pSettlementInfoConfirm->BrokerID, sizeof(TThostFtdcBrokerIDType));
				strncpy(field.InvestorID, pSettlementInfoConfirm->InvestorID, sizeof(TThostFtdcInvestorIDType));
				strncpy(field.TradingDay, pSettlementInfoConfirm->ConfirmDate, sizeof(TThostFtdcDateType));

				api->ReqQrySettlementInfo(&field, requestID++);
			}
			else {
				//今天已经确认
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

///请求查询投资者结算结果响应
void CtpTD::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	cout << "请求查询投资者结算结果响应, ,  ID: " << nRequestID << endl;
	cout << pSettlementInfo->Content << endl;
	if (pRspInfo == nullptr || pRspInfo->ErrorID == 0) {
		if (bIsLast == true) {
			//确认投资者结算结果
			CThostFtdcSettlementInfoConfirmField field;
			memset(&field, 0, sizeof(CThostFtdcQryTradingAccountField));
			strncpy(field.BrokerID, pSettlementInfo->BrokerID, sizeof(TThostFtdcBrokerIDType));
			strncpy(field.InvestorID, pSettlementInfo->InvestorID, sizeof(TThostFtdcInvestorIDType));
			int result = api->ReqSettlementInfoConfirm(&field, requestID++);
			cout << "result:" << result << endl;
		}
	}
	else {
		cout << "错误应答:" << pRspInfo->ErrorID << pRspInfo->ErrorMsg << " 请求ID为" << requestID << endl;
	}
}


void CtpTD::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		if (pSettlementInfoConfirm != nullptr) {
			cout << "经纪公司代码:" << pSettlementInfoConfirm->BrokerID << endl
				<< "用户账号:" << pSettlementInfoConfirm->InvestorID << endl
				<< "确定日期：" << pSettlementInfoConfirm->ConfirmDate << endl
				<< "确定时间：" << pSettlementInfoConfirm->ConfirmTime << endl;

			//查询所有可用合约信息
			CThostFtdcQryInstrumentField myreq = CThostFtdcQryInstrumentField();
			memset(&myreq, 0, sizeof(CThostFtdcQryInstrumentField));
			api->ReqQryInstrument(&myreq, requestID++);
		}
		else {
			cout << "投资者结算结果确认响应为空, pSettlementInfoConfirm:" << pSettlementInfoConfirm << endl;
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



///请求查询合约响应
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
	///经纪公司代码
	strcpy(order.BrokerID, this->login_info->BrokerID);
	///投资者代码
	strcpy(order.InvestorID, this->login_info->UserID);
	strcpy(order.UserID, this->login_info->UserID);
	///报单引用
	sprintf(order.OrderRef, "%d", this->requestID++);
	///用户代码
//	TThostFtdcUserIDType	UserID;
	///报单价格条件: 限价
	//req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;

	///组合投机套保标志
	order.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;

	///GTD日期
//	TThostFtdcDateType	GTDDate;

	//req.VolumeCondition = THOST_FTDC_VC_CV;

	///止损价
//	TThostFtdcPriceType	StopPrice;
	///强平原因: 非强平
	order.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///自动挂起标志: 否
	order.IsAutoSuspend = 0;
	///业务单元
//	TThostFtdcBusinessUnitType	BusinessUnit;
	///请求编号
//	TThostFtdcRequestIDType	RequestID;
	///用户强评标志: 否
	order.UserForceClose = 0;

	int iResult = this->api->ReqOrderInsert(&order, ++requestID);
	cerr << "--->>> 报单录入请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
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

///报单通知
void CtpTD::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	/*
	///经纪公司代码
	TThostFtdcBrokerIDType BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType InvestorID;
	///合约代码
	TThostFtdcInstrumentIDType InstrumentID;
	///报单引用
	TThostFtdcOrderRefType OrderRef;
	///用户代码
	TThostFtdcUserIDType UserID;
	///报单价格条件
	TThostFtdcOrderPriceTypeType OrderPriceType;
	///买卖方向
	TThostFtdcDirectionType Direction;
	///组合开平标志
	TThostFtdcCombOffsetFlagType CombOffsetFlag;
	///组合投机套保标志
	TThostFtdcCombHedgeFlagType CombHedgeFlag;
	///价格
	TThostFtdcPriceType LimitPrice;
	///数量
	TThostFtdcVolumeType VolumeTotalOriginal;
	///有效期类型
	TThostFtdcTimeConditionType TimeCondition;
	///GTD日期
	TThostFtdcDateType GTDDate;
	///成交量类型
	TThostFtdcVolumeConditionType VolumeCondition;
	///最小成交量
	TThostFtdcVolumeType MinVolume;
	///触发条件
	TThostFtdcContingentConditionType ContingentCondition;
	///止损价
	TThostFtdcPriceType StopPrice;
	///强平原因
	TThostFtdcForceCloseReasonType ForceCloseReason;
	///自动挂起标志
	TThostFtdcBoolType IsAutoSuspend;
	///业务单元
	TThostFtdcBusinessUnitType BusinessUnit;
	///请求编号
	TThostFtdcRequestIDType RequestID;
	///本地报单编号
	TThostFtdcOrderLocalIDType OrderLocalID;
	///交易所代码
	TThostFtdcExchangeIDType ExchangeID;
	///会员代码
	TThostFtdcParticipantIDType ParticipantID;
	///客户代码
	TThostFtdcClientIDType ClientID;
	///合约在交易所的代码
	TThostFtdcExchangeInstIDType ExchangeInstID;
	///交易所交易员代码
	TThostFtdcTraderIDType TraderID;
	///安装编号
	TThostFtdcInstallIDType InstallID;
	///报单提交状态
	TThostFtdcOrderSubmitStatusType OrderSubmitStatus;
	///报单提示序号
	TThostFtdcSequenceNoType NotifySequence;
	///交易日
	TThostFtdcDateType TradingDay;
	///结算编号
	TThostFtdcSettlementIDType SettlementID;
	///报单编号
	TThostFtdcOrderSysIDType OrderSysID;
	///报单来源
	TThostFtdcOrderSourceType OrderSource;
	///报单状态
	TThostFtdcOrderStatusType OrderStatus;
	///报单类型
	TThostFtdcOrderTypeType OrderType;
	///今成交数量
	TThostFtdcVolumeType VolumeTraded;
	///剩余数量
	TThostFtdcVolumeType VolumeTotal;
	///报单日期
	TThostFtdcDateType InsertDate;
	///委托时间
	TThostFtdcTimeType InsertTime;
	///激活时间
	TThostFtdcTimeType ActiveTime;
	///挂起时间
	TThostFtdcTimeType SuspendTime;
	///最后修改时间
	TThostFtdcTimeType UpdateTime;
	///撤销时间
	TThostFtdcTimeType CancelTime;
	///最后修改交易所交易员代码
	TThostFtdcTraderIDType ActiveTraderID;
	///结算会员编号
	TThostFtdcParticipantIDType ClearingPartID;
	///序号
	TThostFtdcSequenceNoType SequenceNo;
	///前置编号
	TThostFtdcFrontIDType FrontID;
	///会话编号
	TThostFtdcSessionIDType SessionID;
	///用户端产品信息
	TThostFtdcProductInfoType UserProductInfo;
	///状态信息
	TThostFtdcErrorMsgType StatusMsg;
	///用户强评标志
	TThostFtdcBoolType UserForceClose;
	///操作用户代码
	TThostFtdcUserIDType ActiveUserID;
	///经纪公司报单编号
	TThostFtdcSequenceNoType BrokerOrderSeq;
	///相关报单
	TThostFtdcOrderSysIDType RelativeOrderSysID;
	///郑商所成交数量
	TThostFtdcVolumeType ZCETotalTradedVolume;
	///互换单标志
	TThostFtdcBoolType IsSwapOrder;
	///营业部编号
	TThostFtdcBranchIDType BranchID;
	///投资单元代码
	TThostFtdcInvestUnitIDType InvestUnitID;
	///资金账号
	TThostFtdcAccountIDType AccountID;
	///币种代码
	TThostFtdcCurrencyIDType CurrencyID;
	///IP地址
	TThostFtdcIPAddressType IPAddress;
	///Mac地址
	TThostFtdcMacAddressType MacAddress;
*/
}

void CtpTD::OnRtnTrade(CThostFtdcTradeField * pTrade)
{
	this->__onRtnTrade_vtr.push_back(*pTrade);
	this->__onRtnTrade_vtr.erase(this->__onRtnTrade_vtr.begin());
	//FrontID + SessionID + OrderRef
		/////经纪公司代码
		//TThostFtdcBrokerIDType	BrokerID;
		/////投资者代码
		//TThostFtdcInvestorIDType	InvestorID;
		/////合约代码
		//TThostFtdcInstrumentIDType	InstrumentID;
	/////报单引用
	//TThostFtdcOrderRefType	OrderRef;
		/////用户代码
		//TThostFtdcUserIDType	UserID;
		/////交易所代码
		//TThostFtdcExchangeIDType	ExchangeID;
		/////成交编号
		//TThostFtdcTradeIDType	TradeID;
		/////买卖方向
		//TThostFtdcDirectionType	Direction;
	/////报单编号
	//TThostFtdcOrderSysIDType	OrderSysID;
		/////会员代码
		//TThostFtdcParticipantIDType	ParticipantID;
		/////客户代码
		//TThostFtdcClientIDType	ClientID;
		/////交易角色
		//TThostFtdcTradingRoleType	TradingRole;
		/////合约在交易所的代码
		//TThostFtdcExchangeInstIDType	ExchangeInstID;
		/////开平标志
		//TThostFtdcOffsetFlagType	OffsetFlag;
		/////投机套保标志
		//TThostFtdcHedgeFlagType	HedgeFlag;
		/////价格
		//TThostFtdcPriceType	Price;
		/////数量
		//TThostFtdcVolumeType	Volume;
		/////成交时期
		//TThostFtdcDateType	TradeDate;
		/////成交时间
		//TThostFtdcTimeType	TradeTime;
		/////成交类型
		//TThostFtdcTradeTypeType	TradeType;
		/////成交价来源
		//TThostFtdcPriceSourceType	PriceSource;
		/////交易所交易员代码
		//TThostFtdcTraderIDType	TraderID;
		/////本地报单编号
		//TThostFtdcOrderLocalIDType	OrderLocalID;
		/////结算会员编号
		//TThostFtdcParticipantIDType	ClearingPartID;
		/////业务单元
		//TThostFtdcBusinessUnitType	BusinessUnit;
		/////序号
		//TThostFtdcSequenceNoType	SequenceNo;
		/////交易日
		//TThostFtdcDateType	TradingDay;
		/////结算编号
		//TThostFtdcSettlementIDType	SettlementID;
		/////经纪公司报单编号
		//TThostFtdcSequenceNoType	BrokerOrderSeq;
		/////成交来源
		//TThostFtdcTradeSourceType	TradeSource;
		/////投资单元代码
		//TThostFtdcInvestUnitIDType	InvestUnitID;

}

int CtpTD::ReqOrderAction(CThostFtdcInputOrderActionField pInputOrderAction)
{
	this->requestID++;
	///经纪公司代码
	strcpy(pInputOrderAction.BrokerID, this->login_info->BrokerID);
	///投资者代码
	strcpy(pInputOrderAction.InvestorID, this->login_info->UserID);
	strcpy(pInputOrderAction.UserID, this->login_info->UserID);
	///报单操作引用
	pInputOrderAction.OrderActionRef = this->requestID;
	///前置编号
	pInputOrderAction.FrontID = this->userLogin_RSP->FrontID;
	///会话编号
	pInputOrderAction.SessionID = this->userLogin_RSP->SessionID;


	return requestID;
}

vector<CThostFtdcTradeField>* CtpTD::OnRtnTradeVtr()
{
	return &this->__onRtnTrade_vtr;
}

