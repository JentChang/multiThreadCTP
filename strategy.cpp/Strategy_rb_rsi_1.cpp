#include "Strategy_rb_rsi_1.h"



Strategy_rb_rsi::Strategy_rb_rsi(TDEvent* tdevent):
	StrategyTemplate(tdevent)
{
	Strategy_rb_rsi::FILEW.open("E:\\IWORK\\CPPproject\\CTP\\CTP_Multi_Thread\\build\\data.json\\rbs.csv");
	Strategy_rb_rsi::FILEW.close();
}

Strategy_rb_rsi::~Strategy_rb_rsi()
{

}

void Strategy_rb_rsi::InIt()
{
	this->__InstrumentID = "rb2105";
	this->indn->fre(10);
}

void Strategy_rb_rsi::ReceiveTick(TickInfomation tick)
{
	StrategyTemplate::__tick = tick;
	//this->indn->make_bar(tick);

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


	//BarInfomation* bar = this->indn->bar();
	//if (this->indn->barclose())
	//{
	//	this->OnBar(bar);
	//}

}

void Strategy_rb_rsi::OnBar(BarInfomation * bar)
{
	std::cout << bar->TradingDay << " "
		<< bar->UpdateTime << " "
		<< bar->InstrumentID
		<< endl;
}
