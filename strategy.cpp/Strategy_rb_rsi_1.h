#pragma once

#ifndef _STRATEGY_RB_RSI_H_
#define _STRATEGY_RB_RSI_H_


#include "StrategyTemplate.h"


class Strategy_rb_rsi:
	public StrategyTemplate
{
public:
	Strategy_rb_rsi(TDEvent*);
	~Strategy_rb_rsi();

public:
	void InIt();
	void ReceiveTick(TickInfomation);
	void OnBar(BarInfomation* bar);

private:
	ofstream FILEW;


};















#endif 