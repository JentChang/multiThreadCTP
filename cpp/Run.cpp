#include "CtpTD.h"
#include "CtpMD.h"

#include "DataStruct.h"
#include "Strategy_rb_rsi_1.h"

using namespace rapidjson;

using namespace std;


int main(int argc, char* argv[]) {
	ifstream ifs("../data.json/ctp_config/ctp_config.json");
	IStreamWrapper isw(ifs);

	Document docRead;
	docRead.ParseStream(isw);
	if (docRead.HasParseError()) {
		std::cout << "readFromJson error" << std::endl;
	}

	LoginInfo login_info;
	strcpy(login_info.BrokerID, docRead["brokerID"].GetString());
	strcpy(login_info.UserID, docRead["username"].GetString());
	strcpy(login_info.Password, docRead["password"].GetString());
	strcpy(login_info.FRONT_ADDR_MD, docRead["mdUrl"].GetString());
	strcpy(login_info.FRONT_ADDR_TD, docRead["tdUrl"].GetString());

	CThostFtdcTraderApi* tdapi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	CThostFtdcReqUserLoginField* loginField = new CThostFtdcReqUserLoginField();

	strncpy(loginField->UserID, login_info.UserID, sizeof(loginField->UserID));
	strncpy(loginField->Password, login_info.Password, sizeof(loginField->Password));
	strncpy(loginField->BrokerID, login_info.BrokerID, sizeof(loginField->BrokerID));

	std::string filePath = "../data.json/instruments/instruments.json";
	
	///注册td
	CtpTD* td = new CtpTD(tdapi, loginField);
	///验证
	td->InLoginInfo(&login_info);
	tdapi->RegisterSpi(td);

	///公有私有
	tdapi->SubscribePublicTopic(THOST_TERT_RESTART);
	tdapi->SubscribePrivateTopic(THOST_TERT_RESTART);

	tdapi->RegisterFront(login_info.FRONT_ADDR_TD);


	tdapi->Init();
	std::this_thread::sleep_for(std::chrono::seconds(10));

	
	///mdevent 行情推送
	MDEvent* mdevent = new MDEvent();
	///start md
	//	std::vector<CThostFtdcInstrumentField> instruments;
	//readFromJson(instruments, filePath);
	CThostFtdcMdApi* mdapi = CThostFtdcMdApi::CreateFtdcMdApi();
	CtpMD* md = new CtpMD(mdapi, loginField, mdevent);

	mdapi->RegisterSpi(md);
	
	mdapi->RegisterFront(login_info.FRONT_ADDR_MD);
	mdapi->Init();
	std::this_thread::sleep_for(std::chrono::seconds(2));


	char** insts = new char*[3];
	insts[0] = "rb2105";
	insts[1] = "hc2105";
	insts[2] = "m2105";

	int ret = mdapi->SubscribeMarketData(insts, 3);
	mdevent->InsNum(3);


	TDEvent* tdevent = new TDEvent(td);
	///注册策略
	StrategyTemplate* st = new Strategy_rb_rsi(tdevent);
	mdevent->AddStrategy(st);
	///启动
	mdevent->StrategyStart();


	//tdapi->Join();
	//mdapi->Join();
	//mdevent->join();
	while (true)
	{

	}

	return 0;
}