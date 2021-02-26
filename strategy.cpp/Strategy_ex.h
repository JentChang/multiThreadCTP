#pragma once

#ifndef _Strategy_ex_H_
#define _Strategy_ex_H_


#include "StrategyTemplate.h"


class Strategy_ex:
	public StrategyTemplate
{
public:
	Strategy_ex(TDEvent*);
	~Strategy_ex();

public:
	void InIt();
	void ReceiveTick(TickInfomation);
	void OnBar(BarInfomation* bar);
	///½»Ò×
	void trading(TickInfomation tick, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType symbol);
	///
	void rtn_order(CThostFtdcOrderField* rtnOrder, TThostFtdcVolumeType volume);

private:
	bool __long__ = false;
	bool __short__ = false;
	bool __sell__ = false;
	bool __cover__ = false;

	int __pos__ = 0;


};

#endif 