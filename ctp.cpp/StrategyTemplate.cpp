#include "StrategyTemplate.h"

StrategyTemplate::StrategyTemplate(TDEvent* tdevent)
{
	this->tdevent = tdevent;
	this->indn = new Indicators();
	this->__tick = TickInfomation();
}

StrategyTemplate::~StrategyTemplate()
{
	delete this->indn;
	this->__InstrumentID = nullptr;
	this->indn = nullptr;
}

void StrategyTemplate::InIt()
{
	///重写InIt
	///策略初始化 参数等设计
}

void StrategyTemplate::ReceiveTick(TickInfomation tick)
{
	///重写RiceiverTick 
	///不使用tick指针 它是一个时刻更新的量
	StrategyTemplate::__tick = tick;
	this->indn->make_bar(tick);
	BarInfomation* bar = this->indn->bar();

	///这里可以新增OnBar函数处理k线信息
	if (this->indn->barclose())
	{
		///this->OnBar(bar);
	}


	return this->FunEnd();
}

void StrategyTemplate::OnBar(BarInfomation * bar)
{
}

void StrategyTemplate::FunStart()
{
	this->funend = false;
}

bool StrategyTemplate::FunState()
{
	return this->funend;
}


TickInfomation StrategyTemplate::GetTick()
{
	return StrategyTemplate::__tick;
}


char* StrategyTemplate::InstrumentID()
{
	return this->__InstrumentID;
}

void StrategyTemplate::FunEnd()
{
	this->funend = true;
}



int StrategyTemplate::LONG(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID)
{
	return this->tdevent->LONG(price, volume, InstrumentID);
}

int StrategyTemplate::SELL(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID)
{
	return this->tdevent->SELL(price, volume, InstrumentID);
}

int StrategyTemplate::SHORT(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID)
{
	return this->tdevent->SHORT(price, volume, InstrumentID);
}

int StrategyTemplate::COVER(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID)
{
	return this->tdevent->COVER(price, volume, InstrumentID);
}