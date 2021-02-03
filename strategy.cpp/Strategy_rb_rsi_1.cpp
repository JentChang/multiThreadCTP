#include "Strategy_rb_rsi_1.h"



Strategy_rb_rsi::Strategy_rb_rsi(TDEvent* tdevent, Indicators*indn):
	StrategyTemplate(tdevent, indn)
{
	
}

Strategy_rb_rsi::~Strategy_rb_rsi()
{
}

void Strategy_rb_rsi::InIt()
{
}

void Strategy_rb_rsi::ReceiveTick(TickInfomation tick)
{
	StrategyTemplate::__tick = tick;
	this->indn->make_bar(tick);
	BarInfomation* bar = this->indn->bar();
}
