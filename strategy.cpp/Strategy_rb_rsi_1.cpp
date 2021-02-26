#include "Strategy_rb_rsi_1.h"



Strategy_rb_rsi::Strategy_rb_rsi(TDEvent* tdevent):
	StrategyTemplate(tdevent)
{
	///this->indn = new Indicators(); ���������ʵ��
	this->indn = new Indicators();
	Strategy_rb_rsi::FILEW.open("E:\\IWORK\\CPPproject\\CTP\\CTP_Multi_Thread\\build\\data.json\\rbs.csv");
}

Strategy_rb_rsi::~Strategy_rb_rsi()
{
	delete this->indn;
	Strategy_rb_rsi::FILEW.close();
}

void Strategy_rb_rsi::InIt()
{
	this->__InstrumentID = "rb2105";
}

void Strategy_rb_rsi::ReceiveTick(TickInfomation tick)
{
	StrategyTemplate::__tick = tick;
	this->indn->fre(1);
	this->indn->make_bar(tick);

	string day = tick.TradingDay;
	string time = tick.UpdateTime;
	string mtime = to_string(tick.UpdateMillisec);

	std::cout << tick.datetime << " "
		<< tick.InstrumentID << " "
		<< day << " "
		<< time << " "
		<< mtime << " "
		<< endl;
	Strategy_rb_rsi::FILEW.open("E:\\IWORK\\CPPproject\\CTP\\CTP_Multi_Thread\\build\\data.json\\rbs.csv", ios::app);
	Strategy_rb_rsi::FILEW << tick.datetime << " "
		<< tick.InstrumentID << " "
		<< day << " "
		<< time << " "
		<< mtime << " "
		<< endl;
	Strategy_rb_rsi::FILEW.close();


	BarInfomation* bar = this->indn->bar();
	if (this->indn->barclose())
	{
		this->OnBar(bar);
	}
	if (this->istest)
	{
		//CThostFtdcOrderField pOrder;
		//pOrder = this->LONG(4520, 1, "rb2105");
		//std::cout << "order id : " << pOrder.OrderSysID << endl;
		//std::cout << " �ɽ����� " << pOrder.VolumeTraded << " ʣ������ " << pOrder.VolumeTotal << endl;
		//int action_rtn = this->tdevent->ACTION(pOrder.OrderSysID, "SHFE", this->__InstrumentID);
		//std::cout << "action: " << action_rtn << endl;

		///�ʽ�
		CThostFtdcTradingAccountField acc = this->tdevent->Account();
		///�ֲ�
		CThostFtdcInvestorPositionField pos = this->tdevent->Postion("SHFE", this->__InstrumentID);

		if (strcmp(acc.TradingDay, ACC_TRADING_DAY_EMPYT_FLAG) == 0)
		{
			std::cout << "�ʽ��ѯ���� nullpter" << endl;
		}
		else if (strcmp(acc.TradingDay, ACC_TRADING_DAY_ERROR) == 0)
		{
			std::cout << "�ʽ��ѯ�������" << endl;
		}
		else {
			std::cout << " �����ʽ�: " << acc.Available << " ��ǰ��֤���ܶ�: " << acc.CurrMargin << endl;
		}

		if (pos.Position == POSITION_EMPYT_FLAG)
		{
			std::cout << "�ֲֲ�ѯ���� nullpter" << endl;
		}
		else if (pos.Position == POSITION_EMPYT_FLAG)
		{
			std::cout << "�ֲֲ�ѯ�������" << endl;
		}
		else {
			std::cout << " ���ճֲ�: " << pos.Position << " �ֲַ���: " << pos.PosiDirection << endl;
		}
		this->istest = false;
	}
}

void Strategy_rb_rsi::OnBar(BarInfomation * bar)
{

	///�ʽ�
	CThostFtdcTradingAccountField acc = this->tdevent->Account();
	///�ֲ�
	CThostFtdcInvestorPositionField pos = this->tdevent->Postion("SHFE", this->__InstrumentID);
	if (strcmp(acc.TradingDay, ACC_TRADING_DAY_EMPYT_FLAG) == 0)
	{
		std::cout << "�ʽ��ѯ���� nullpter" << endl;
	}
	else if (strcmp(acc.TradingDay, ACC_TRADING_DAY_ERROR) == 0)
	{
		std::cout << "�ʽ��ѯ�������" << endl;
	}
	else {
		std::cout << " �����ʽ�: " << acc.Available << " ��ǰ��֤���ܶ�: " << acc.CurrMargin << endl;
	}

	if (pos.Position == POSITION_EMPYT_FLAG)
	{
		std::cout << "�ֲֲ�ѯ���� nullpter" << endl;
	}
	else if (pos.Position == POSITION_EMPYT_FLAG)
	{
		std::cout << "�ֲֲ�ѯ�������" << endl;
	}
	else {
		std::cout << " ���ճֲ�: " << pos.Position << " �ֲַ���: " << pos.PosiDirection << endl;
	}
	this->istest = false;
}


