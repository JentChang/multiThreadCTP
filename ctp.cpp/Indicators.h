#pragma once

#ifndef _INDICATOR_H_
#define _INDICATOR_H_

#include "DataStruct.h"

class Indicators
{
public:
	Indicators();
	~Indicators();

public:
	void make_bar(TickInfomation);
	BarInfomation* bar();
	bool barclose();
	void fre(int);

	///indicator
	double average(double* arr, int size);
	double std(double* arr, int size);

	double RSI(double close, int peridos = 14);
	double ATR(double high, double low, double close, int timeperiod = 14);
	double max_arr(double* arr, int size);
	double ATR_compire_AATR(double atr, int timeperiod);

private:
	TickInfomation __tick_pre__;
	BarInfomation __bar__;
	int __updateTime__ = 1;
	int __updateH__ = 9;

	int __fre__ = 15;

	bool __barclose__=false;
	BarInfomation __newBar__;

	double __i__ = 0;
	double __j__ = 0;
	bool __BS__ = true;
	vector<string> time_strs;


	////indicator
	double __rsi_proitive_mena = 0;
	double __rsi_negative_mena = 0;
	double __rsi_close_pre = 0;
	int __rsi_count_num = 0;

	double __atr_close_pre = 0;
	double __atr_atr = 0;
	int __atr_count_num = 0;


	double __aatr_aatr = 0;
	double __aatr_sum = 0;
	double __aatr_atr;
	int __aatr_count_num = 0;
};
#endif