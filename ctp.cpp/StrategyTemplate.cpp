#include "StrategyTemplate.h"

StrategyTemplate::StrategyTemplate(TDEvent* tdevent, Indicators*indn)
{
	this->tdevent = tdevent;
	this->indn = indn;
}

StrategyTemplate::~StrategyTemplate()
{
	this->__InstrumentID = nullptr;
	this->indn = nullptr;
}

void StrategyTemplate::InIt()
{
	this->__InstrumentID = "rb2105";
}

void StrategyTemplate::ReceiveTick(TickInfomation tick)
{
	///��ʹ��tickָ�� ����һ��ʱ�̸��µ���
	StrategyTemplate::__tick = tick;
	this->indn->make_bar(tick);
	BarInfomation* bar = this->indn->bar();



	return this->FunEnd();
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