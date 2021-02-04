#include "Strategy_rb_rsi_1.h"



Strategy_rb_rsi::Strategy_rb_rsi(TDEvent* tdevent):
	StrategyTemplate(tdevent)
{
	
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
	this->indn->make_bar(tick);

	std::cout << tick.datetime << " "
		<< tick.InstrumentID
		<< endl;

	BarInfomation* bar = this->indn->bar();
	if (this->indn->barclose())
	{
		this->OnBar(bar);
	}

}

void Strategy_rb_rsi::OnBar(BarInfomation * bar)
{
	std::cout << bar->TradingDay << " "
		<< bar->UpdateTime << " "
		<< bar->InstrumentID
		<< endl;
}
