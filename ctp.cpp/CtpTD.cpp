#include "CtpTD.h"
#include <Windows.h>



CtpTD::CtpTD(CThostFtdcTraderApi* api, CThostFtdcReqUserLoginField* pUserLogin) :
	api(api), userLogin(pUserLogin)
{
	this->requestID = 1;
	this->__onRtnTrade_vtr.clear();
	this->__onRtnTrade_vtr.resize(500);

	this->rtn_order = new CThostFtdcOrderField();
	this-> rtn_position = new CThostFtdcInvestorPositionField();
	this->rtn_acc = new CThostFtdcTradingAccountField();
}


CtpTD::~CtpTD()
{
	this->api = nullptr;
	this->userLogin = nullptr;
	this->userLogin_RSP = nullptr;
	delete this->rtn_order;
	delete this->rtn_position;
	delete this->rtn_acc;
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
		LOG("ReqAuthenticate response: [%d]\n", AuthRet);
	}
}

void CtpTD::OnFrontDisconnected(int nReason)
{
	LOG("OnFrontDisconnected, nReason: [%d]\n", nReason)
	//std::cout << "OnFrontDisconnected, nReason:" << nReason << std::endl;
}

void CtpTD::OnRspAuthenticate(CThostFtdcRspAuthenticateField * pRspAuthenticateField, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		LOG("OnRspAuthenticate ok\n");
		//std::cout << "OnRspAuthenticate ok" << std::endl;
		api->ReqUserLogin(userLogin, requestID++);
	}
	else
	{
		LOG("OnRspAuthenticate ERROR : [%d %s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		//std::cout << "OnRspAuthenticate error:"
		//	<< pRspInfo->ErrorID
		//	<< pRspInfo->ErrorMsg
		//	<< std::endl;
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
}

///登录请求响应
void CtpTD::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		LOG("OnRspUserLogin ok\n")
		//std::cout << "OnRspUserLogin ok" << std::endl;
		this->tradingDate = api->GetTradingDay();

		this->userLogin_RSP = pRspUserLogin;

		LOG("TradingDay: [%s]\n", tradingDate.c_str());
		LOG("登录成功. \n");
		LOG("前置编号: [%d]\n", pRspUserLogin->FrontID);
		LOG("会话编号: [%d]\n", pRspUserLogin->SessionID);
		LOG("最大报单引用: [%s]\n", pRspUserLogin->MaxOrderRef);
		LOG("上期所时间: [%s]\n", pRspUserLogin->SHFETime);
		LOG("大商所时间: [%s]\n", pRspUserLogin->DCETime);
		LOG("郑商所时间: [%s]\n", pRspUserLogin->CZCETime);
		LOG("中金所时间: [%s]\n", pRspUserLogin->FFEXTime);
		LOG("能源中心时间: [%s]\n", pRspUserLogin->INETime);

		//std::cout << "TradingDay:" << tradingDate.c_str() << std::endl;
		//cout << "登录成功. " << endl
		//	<< "前置编号:" << pRspUserLogin->FrontID << endl
		//	<< "会话编号" << pRspUserLogin->SessionID << endl
		//	<< "最大报单引用:" << pRspUserLogin->MaxOrderRef << endl
		//	<< "上期所时间：" << pRspUserLogin->SHFETime << endl
		//	<< "大商所时间：" << pRspUserLogin->DCETime << endl
		//	<< "郑商所时间：" << pRspUserLogin->CZCETime << endl
		//	<< "中金所时间：" << pRspUserLogin->FFEXTime << endl
		//	<< "能源中心时间：" << pRspUserLogin->INETime << endl;
		
		//向期货商第一次发送交易指令前，需要先查询投资者结算结果， 投资者确认以后才能交易。
		//查询是否已经做了确认
		CThostFtdcSettlementInfoConfirmField pSettlementInfoConfirm;
		memset(&pSettlementInfoConfirm, 0, sizeof(CThostFtdcSettlementInfoConfirmField));
		strncpy(pSettlementInfoConfirm.BrokerID, pRspUserLogin->BrokerID, sizeof(TThostFtdcBrokerIDType));
		strncpy(pSettlementInfoConfirm.InvestorID, pRspUserLogin->UserID, sizeof(TThostFtdcInvestorIDType));
		api->ReqSettlementInfoConfirm(&pSettlementInfoConfirm, requestID++);
	}
	else {
		LOG("OnRspUserLogin ERROR: [%d %s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		//std::cout << "OnRspUserLogin error:"
		//	<< pRspInfo->ErrorID
		//	<< pRspInfo->ErrorMsg
		//	<< std::endl;
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
}


///请求查询结算信息确认响应
void CtpTD::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		if (nullptr == pSettlementInfoConfirm)
		{
			LOG("OnRspQrySettlementInfoConfirm: [%s %s]\n", pSettlementInfoConfirm->ConfirmDate, pSettlementInfoConfirm->ConfirmTime);
			//std::cout << "OnRspQrySettlementInfoConfirm:"
			//	<< pSettlementInfoConfirm->ConfirmDate << ","
			//	<< pSettlementInfoConfirm->ConfirmTime << ","
			//	<< bIsLast
			//	<< std::endl;


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
		LOG("OnRspQrySettlementInfoConfirm ERROR: [%d, %s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		//std::cout << "OnRspQrySettlementInfoConfirm error:"
		//	<< pRspInfo->ErrorID
		//	<< pRspInfo->ErrorMsg
		//	<< std::endl;
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
}

///请求查询投资者结算结果响应
void CtpTD::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	LOG("请求查询投资者结算结果响应, ,  ID: [ %d %s]\n", nRequestID, pSettlementInfo->Content);
	//cout << "请求查询投资者结算结果响应, ,  ID: " << nRequestID << endl;
	//cout << pSettlementInfo->Content << endl;
	if (pRspInfo == nullptr || pRspInfo->ErrorID == 0) {
		if (bIsLast == true) {
			//确认投资者结算结果
			CThostFtdcSettlementInfoConfirmField field;
			memset(&field, 0, sizeof(CThostFtdcQryTradingAccountField));
			strncpy(field.BrokerID, pSettlementInfo->BrokerID, sizeof(TThostFtdcBrokerIDType));
			strncpy(field.InvestorID, pSettlementInfo->InvestorID, sizeof(TThostFtdcInvestorIDType));
			int result = api->ReqSettlementInfoConfirm(&field, requestID++);
			LOG("result: [%d]\n", result);
			//cout << "result:" << result << endl;
		}
	}
	else {
		LOG("错误应答: [%d %s]  请求ID为 [%d]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, requestID);
		//cout << "错误应答:" << pRspInfo->ErrorID << pRspInfo->ErrorMsg << " 请求ID为" << requestID << endl;
	}
}


void CtpTD::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		if (pSettlementInfoConfirm != nullptr) {
			LOG("经纪公司代码: [%s]\n", pSettlementInfoConfirm->BrokerID);
			LOG("用户账号: [%s]\n", pSettlementInfoConfirm->InvestorID);
			LOG("确定日期: [%s]\n", pSettlementInfoConfirm->ConfirmDate);
			LOG("确定时间: [%s]\n", pSettlementInfoConfirm->ConfirmTime);

			//cout << "经纪公司代码:" << pSettlementInfoConfirm->BrokerID << endl
			//	<< "用户账号:" << pSettlementInfoConfirm->InvestorID << endl
			//	<< "确定日期：" << pSettlementInfoConfirm->ConfirmDate << endl
			//	<< "确定时间：" << pSettlementInfoConfirm->ConfirmTime << endl;

			//查询所有可用合约信息
			CThostFtdcQryInstrumentField myreq = CThostFtdcQryInstrumentField();
			memset(&myreq, 0, sizeof(CThostFtdcQryInstrumentField));
			api->ReqQryInstrument(&myreq, requestID++);
		}
		else {
			LOG("投资者结算结果确认响应为空, pSettlementInfoConfirm:\n");
			//cout << "投资者结算结果确认响应为空, pSettlementInfoConfirm:" << pSettlementInfoConfirm << endl;
		}
	}
	else {
		LOG("OnRspSettlementInfoConfirm ERROR: [%d %s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		//std::cout << "OnRspSettlementInfoConfirm error:"
		//	<< pRspInfo->ErrorID
		//	<< pRspInfo->ErrorMsg
		//	<< std::endl;
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
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
		LOG("OnRspQryInstrument ERROR: [%d %s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		//std::cout << "OnRspQryInstrument error:"
		//	<< pRspInfo->ErrorID
		//	<< pRspInfo->ErrorMsg
		//	<< std::endl;
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
}

void CtpTD::InLoginInfo(LoginInfo* login_info)
{
	this->login_info = login_info;
}

CThostFtdcOrderField CtpTD::ReqOrderInsert(CThostFtdcInputOrderField order)
{
	CThostFtdcInputOrderField* order_insert = new CThostFtdcInputOrderField();
	memset(order_insert, 0, sizeof(CThostFtdcInputOrderField));

	//strcpy(order.InstrumentID, InstrumentID);

	//order.Direction = THOST_FTDC_D_Buy;
	//order.CombOffsetFlag[0] = THOST_FTDC_OF_Open;

	order_insert->LimitPrice = order.LimitPrice;
	order_insert->VolumeTotalOriginal = order.VolumeTotalOriginal;
	strcpy(order_insert->InstrumentID, order.InstrumentID);
	order_insert->Direction = order.Direction;
	order_insert->CombOffsetFlag[0] = order.CombOffsetFlag[0];

	///经纪公司代码
	strcpy(order_insert->BrokerID, this->login_info->BrokerID);
	///投资者代码
	strcpy(order_insert->InvestorID, this->login_info->UserID);
	strcpy(order_insert->UserID, this->login_info->UserID);
	///报单引用
	sprintf(order_insert->OrderRef, "%d", this->requestID++);
	///用户代码
//	TThostFtdcUserIDType	UserID;
	///报单价格条件: 限价
	order_insert->OrderPriceType = THOST_FTDC_OPT_LimitPrice;

	///组合投机套保标志
	order_insert->CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;

	///GTD日期
	//TThostFtdcDateType	GTDDate;

	///有效期类型: 当日有效
	order_insert->TimeCondition = THOST_FTDC_TC_GFD;///IOC
	///成交量类型: 任何数量
	order_insert->VolumeCondition = THOST_FTDC_VC_AV;//CV
	///最小成交量: 1
	order_insert->MinVolume = 1;
	///触发条件: 立即
	order_insert->ContingentCondition = THOST_FTDC_CC_Immediately;

	///止损价
	//	TThostFtdcPriceType	StopPrice;
	///强平原因: 非强平
	order_insert->ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///自动挂起标志: 否
	order_insert->IsAutoSuspend = 0;
	///业务单元
//	TThostFtdcBusinessUnitType	BusinessUnit;
	///请求编号
//	TThostFtdcRequestIDType	RequestID;
	///用户强评标志: 否
	order_insert->UserForceClose = 0;

	memset(this->rtn_order, 0, sizeof(CThostFtdcOrderField));
	this->rtn_order->RequestID = RTN_ORDER_INIT;
	int iResult = this->api->ReqOrderInsert(order_insert, ++requestID);
	delete order_insert;

	LOG("order requestID: [%D]\n", requestID - 1);
	LOG("--->>> 报单录入请求: ");
	LOG((iResult == 0) ? "成功\n" : "失败\n");
	//std::cout << "order requestID: " << requestID - 1;
	//cerr << "--->>> 报单录入请求: " << ((iResult == 0) ? "成功" : "失败") << endl;

	if (iResult != 0)
	{
		this->rtn_order->RequestID = TN_ORDER_ERROR;
		return *this->rtn_order;
	}

	while (this->rtn_order->RequestID == RTN_ORDER_INIT)
	{

	}
	LOG(" order id : [%s]\n", this->rtn_order->OrderSysID);
	//std::cout << " order id : " << this->rtn_order->OrderSysID << endl;

	return 	*this->rtn_order;
}

void CtpTD::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LOG("--->>> OnRspOrderInsert\n");
	//cerr << "--->>> " << "OnRspOrderInsert" << endl;
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
		LOG("OnRspOrderInsert ERROR: [%d %s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		//std::cout << "OnRspOrderInsert error:"
		//	<< pRspInfo->ErrorID << ", "
		//	<< pRspInfo->ErrorMsg
		//	<< std::endl;
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
}

///报单通知
void CtpTD::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	memcpy(this->rtn_order, pOrder, sizeof(CThostFtdcOrderField));
}

void CtpTD::OnRtnTrade(CThostFtdcTradeField * pTrade)
{
	this->__onRtnTrade_vtr.push_back(*pTrade);
	this->__onRtnTrade_vtr.erase(this->__onRtnTrade_vtr.begin());
}

void CtpTD::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField * pInvestorPosition, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		if (bIsLast || pInvestorPosition == nullptr)
		{
			this->rtn_position->Position = POSITION_EMPYT_FLAG;
		}
		else {
			memcpy(this->rtn_position, pInvestorPosition, sizeof(CThostFtdcInvestorPositionField));
		}
	}
	else {
		this->rtn_position->Position = POSITION_ERROR;
		LOG("OnRspQryTradingAccount ERROR: [%d %s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		//std::cout << "OnRspQryTradingAccount error:"
		//	<< pRspInfo->ErrorID
		//	<< pRspInfo->ErrorMsg
		//	<< std::endl;
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
}

void CtpTD::OnRspQryTradingAccount(CThostFtdcTradingAccountField * pTradingAccount, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		if (bIsLast || pTradingAccount == nullptr)
		{
			strcpy(this->rtn_acc->TradingDay, ACC_TRADING_DAY_EMPYT_FLAG);
		}
		else {
			memcpy(this->rtn_acc, pTradingAccount, sizeof(CThostFtdcTradingAccountField));
		}
	}
	else {
		strcpy(this->rtn_acc->TradingDay, ACC_TRADING_DAY_ERROR);
		LOG("OnRspQryTradingAccount ERROR: [%d %s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		//std::cout << "OnRspQryTradingAccount error:"
		//	<< pRspInfo->ErrorID
		//	<< pRspInfo->ErrorMsg
		//	<< std::endl;
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
}

int CtpTD::ReqOrderAction(CThostFtdcInputOrderActionField pInputOrderAction)
{
	///经纪公司代码
	strcpy(pInputOrderAction.BrokerID, this->login_info->BrokerID);
	///投资者代码
	strcpy(pInputOrderAction.InvestorID, this->login_info->UserID);
	strcpy(pInputOrderAction.UserID, this->login_info->UserID);
	///前置编号
	pInputOrderAction.FrontID = this->userLogin_RSP->FrontID;
	///会话编号
	pInputOrderAction.SessionID = this->userLogin_RSP->SessionID;

	pInputOrderAction.ActionFlag = THOST_FTDC_AF_Delete;
	int iResult = this->api->ReqOrderAction(&pInputOrderAction, this->requestID++);
	std::cout << "action order requestID: " << requestID;
	LOG("--->>> 撤单录入请求: [%s]\n", ((iResult == 0) ? "成功" : "失败"));
	//cerr << "--->>> 撤单录入请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
	if (iResult == 0)
	{
		return requestID;
	}
	else
	{
		return -1;
	}
}

CThostFtdcInvestorPositionField CtpTD::ReqQryInvestorPosition(CThostFtdcQryInvestorPositionField pQryInvestorPosition)
{
	///交易所代码
	///合约代码
	///经纪公司代码
	strcpy(pQryInvestorPosition.BrokerID, this->login_info->BrokerID);
	///投资者代码
	strcpy(pQryInvestorPosition.InvestorID, this->login_info->UserID);


	memset(this->rtn_position, 0, sizeof(CThostFtdcInvestorPositionField));
	this->rtn_position->Position = POSITION_INIT;
	int iResult = this->api->ReqQryInvestorPosition(&pQryInvestorPosition, requestID++);
	LOG("--->>> 查询持仓请求: [%s]\n", ((iResult == 0) ? "成功" : "失败"));
	//cerr << "--->>> 查询持仓请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
	if (iResult != 0)
	{
		this->rtn_position->Position = POSITION_ERROR;
		return *this->rtn_position;
	}
	while (this->rtn_position->Position == POSITION_INIT)
	{

	}
	return 	*this->rtn_position;
}

CThostFtdcTradingAccountField CtpTD::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField pQryTradingAccount;
	memset(&pQryTradingAccount, 0, sizeof(CThostFtdcQryTradingAccountField));

	/////经纪公司代码
	//strcpy(pQryTradingAccount.BrokerID, this->login_info->BrokerID);
	/////投资者代码
	//strcpy(pQryTradingAccount.InvestorID, this->login_info->UserID);
	/////投资者帐号
	//strcpy(pQryTradingAccount.AccountID, this->login_info->UserID);

	memset(this->rtn_acc, 0, sizeof(CThostFtdcTradingAccountField));
	strcpy(this->rtn_acc->TradingDay, ACC_TRADING_DAY_INIT);
	int iResult = this->api->ReqQryTradingAccount(&pQryTradingAccount, requestID++);
	LOG("--->>> 查询资金请求: [%s]\n", ((iResult == 0) ? "成功" : "失败"));
	//cerr << "--->>> 查询资金请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
	if (iResult != 0)
	{
		strcpy(this->rtn_acc->TradingDay, ACC_TRADING_DAY_ERROR);
		return *this->rtn_acc;
	}
	while (strcmp(this->rtn_acc->TradingDay, ACC_TRADING_DAY_INIT) == 0)
	{

	}
	return 	*this->rtn_acc;
}

vector<CThostFtdcTradeField>* CtpTD::OnRtnTradeVtr()
{
	return &this->__onRtnTrade_vtr;
}

