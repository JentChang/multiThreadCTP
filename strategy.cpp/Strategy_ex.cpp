#include "Strategy_ex.h"


Strategy_ex::Strategy_ex(TDEvent* tdevent):
	StrategyTemplate(tdevent)
{
	this->indn = new Indicators();
	this->__long__ = false;
	this->__short__ = false;
	this->__sell__ = false;
	this->__cover__ = false;
}

Strategy_ex::~Strategy_ex()
{
	delete this->indn;
}

void Strategy_ex::InIt()
{
	this->__InstrumentID = "rb2105";
}

void Strategy_ex::ReceiveTick(TickInfomation tick)
{

	this->trading(tick, 1, tick.InstrumentID);

	StrategyTemplate::__tick = tick;
	this->indn->fre(15);
	this->indn->make_bar(tick);

	BarInfomation* bar = this->indn->bar();
	if (this->indn->barclose())
	{
		this->OnBar(bar);
	}
}

void Strategy_ex::OnBar(BarInfomation * bar)
{
	//重置
	this->__long__ = false;
	this->__short__ = false;
	this->__sell__ = false;
	this->__cover__ = false;

	///实现信号
	if (true)
	{
		///..........
		//this->__long__ = true;
		//this->__short__ = true;
		//this->__sell__ = true;
		//this->__cover__ = true;
	}
	else if (true)
	{
		///...............
	}
	else
	{
		///..............
	}
}


void Strategy_ex::trading(TickInfomation tick, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType symbol)
{
	CThostFtdcOrderField rtnOrder;
	double buyprice = tick.AskPrice1 + xxx;
	double sellprice = tick.BidPrice1 - xxxx;
	if (this->__long__ && !this->__sell__)
	{
		if (tick.AskPrice1 == 0) { return; }

		if (this->__pos__ == 0)
		{
			rtnOrder = this->LONG(buyprice, volume, symbol);
			this->rtn_order(&rtnOrder, volume);
		}
		else if (this->__pos__ < 0)
		{
			rtnOrder = this->COVER(buyprice, volume, symbol);
			this->rtn_order(&rtnOrder, volume);

			rtnOrder = this->LONG(buyprice, volume, symbol);
			this->rtn_order(&rtnOrder, volume);
		}
	}
	else if (this->__short__ && !this->__cover__)
	{
		if (tick.BidPrice1 == 0){return;}

		if (this->__pos__ == 0)
		{
			rtnOrder = this->SHORT(sellprice, volume, symbol);
			this->rtn_order(&rtnOrder, -volume);
		}
		else if (this->__pos__ > 0)
		{
			rtnOrder = this->SELL(sellprice, volume, symbol);
			this->rtn_order(&rtnOrder, -volume);

			rtnOrder = this->SHORT(sellprice, volume, symbol);
			this->rtn_order(&rtnOrder, -volume);
		}
	}
	//平
	else if (this->__pos__ > 0 && this->__sell__)
	{ 
		if (tick.BidPrice1 == 0) { return; } 
		rtnOrder = this->SELL(sellprice, volume, symbol);
		this->rtn_order(&rtnOrder, -volume);
	}
	else if (this->__pos__ < 0 && this->__cover__) 
	{ 
		if (tick.AskPrice1 == 0) { return; } 
		rtnOrder = this->COVER(buyprice, volume, symbol);
		this->rtn_order(&rtnOrder, volume);
	}
}

void Strategy_ex::rtn_order(CThostFtdcOrderField* rtnOrder, TThostFtdcVolumeType volume)
{
	if (rtnOrder->RequestID == TN_ORDER_ERROR)
	{
		std::cout << " 订单录入错误" << endl;
	}
	else if (rtnOrder->VolumeTraded > 0)
	{
		this->__pos__ += rtnOrder->VolumeTraded;
	}
	else if (rtnOrder->VolumeTraded != volume)
	{
		int action_rtn = this->tdevent->ACTION(rtnOrder->OrderSysID, "SHFE", this->__InstrumentID);
	}
}
