#include "CtpTD.h"
#include "CtpMD.h"

#include "DataStruct.h"
#include "Strategy_rb_rsi_1.h"
#include "Strategy_rb_rsi_atr.h"


#define INSTS_COUNT 3

using namespace std;

int main(int argc, char* argv[]) 
{
	CtpTD* m_td;
	CThostFtdcReqUserLoginField* loginField;
	MDEvent* mdevent;
	CtpMD* m_md;
	TDEvent* tdevent;
	StrategyTemplate* st_atr;

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

	CThostFtdcTraderApi* m_tdapi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	loginField = new CThostFtdcReqUserLoginField();

	strncpy(loginField->UserID, login_info.UserID, sizeof(loginField->UserID));
	strncpy(loginField->Password, login_info.Password, sizeof(loginField->Password));
	strncpy(loginField->BrokerID, login_info.BrokerID, sizeof(loginField->BrokerID));

	std::string filePath = "../data.json/instruments/instruments.json";

	///注册td
	m_td = new CtpTD(m_tdapi, loginField);
	///验证
	m_td->InLoginInfo(&login_info);
	m_tdapi->RegisterSpi(m_td);

	///公有私有
	m_tdapi->SubscribePublicTopic(THOST_TERT_RESTART);
	m_tdapi->SubscribePrivateTopic(THOST_TERT_RESTART);

	m_tdapi->RegisterFront(login_info.FRONT_ADDR_TD);

	m_tdapi->Init();
	std::this_thread::sleep_for(std::chrono::seconds(10));

	///mdevent 行情推送
	mdevent = new MDEvent();
	///start md
	//std::vector<CThostFtdcInstrumentField> instruments;
	//readFromJson(instruments, filePath);
	CThostFtdcMdApi* m_mdapi = CThostFtdcMdApi::CreateFtdcMdApi();
	m_md = new CtpMD(m_mdapi, loginField, mdevent);

	m_mdapi->RegisterSpi(m_md);

	m_mdapi->RegisterFront(login_info.FRONT_ADDR_MD);
	m_mdapi->Init();
	std::this_thread::sleep_for(std::chrono::seconds(2));


	char** insts = new char*[3];
	insts[0] = "rb2105";
	//insts[1] = "hc2105";
	//insts[2] = "m2105";
	int ret = m_md->OnSubscribeMarketData(insts, 1);

	std::this_thread::sleep_for(std::chrono::seconds(10));

	tdevent = new TDEvent(m_td);
	///注册策略
	st_atr = new Strategy_rb_rsi_atr(tdevent);
	mdevent->AddStrategy(st_atr);
	///启动
	mdevent->StrategyStart();



	bool IsTradingTime = false;
	bool IsSave = false;
	while (true)
	{
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		//std::cout << sys.wHour << sys.wMinute << endl;
		if (
			(sys.wHour >= 23)||
			(sys.wHour >= 16 && sys.wHour < 20) ||
			(sys.wHour == 8 && sys.wMinute <= 31) ||
			(sys.wHour == 20 && sys.wMinute <= 31) ||
			(sys.wHour == 15 && sys.wMinute >= 30)
			)
		{
			IsTradingTime = false;
		}
		else {
			IsTradingTime = true;
		}

		if (!IsTradingTime)
		{
			if (!IsSave)
			{
				st_atr->indn->save2Json();
				mdevent->StrategyEnd();
				Sleep(2000);
				IsSave = true;
				std::cout << "has saved" << endl;
			}
		}
		else 
		{
			IsSave = false;
		}
		Sleep(2000);
	}
	return 0;
}
