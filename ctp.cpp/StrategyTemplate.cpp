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
	///��дInIt
	///���Գ�ʼ�� ���������
}

void StrategyTemplate::ReceiveTick(TickInfomation tick)
{
	///��дRiceiverTick 
	///��ʹ��tickָ�� ����һ��ʱ�̸��µ���
	StrategyTemplate::__tick = tick;
	this->indn->make_bar(tick);
	BarInfomation* bar = this->indn->bar();

	///�����������OnBar��������k����Ϣ
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