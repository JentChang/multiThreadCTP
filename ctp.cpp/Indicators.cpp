#include "Indicators.h"


Indicators::Indicators()
{
	Indicators::__tick_pre__.EMPTY = true;
	Indicators::__bar__.EMPTY = true;
}

Indicators::~Indicators()
{
}


void Indicators::make_bar(TickInfomation tick)
{
	string bar_time = tick.UpdateTime;
	bar_time = bar_time.substr(0, 5);
	if (bar_time < "09:00" ||
		(bar_time >= "15:00" && bar_time < "21:00") ||
		(bar_time >= "11:30" && bar_time < "13:30") ||
		bar_time >= "23:00" ||  
		(bar_time >= "10:15" & bar_time < "10:30")
		)
	{
		return;
	}
	// 合成k线
	bool newMinitue = false;
	Indicators::__barclose__ = false;
	Indicators::time_strs = Split(bar_time, ":");
	if (time_strs.size() < 2)
	{
		return;
	}
	if (Indicators::__bar__.EMPTY)
	{
		newMinitue = true;
	}
	else
	{
		if (atoi(time_strs[1].c_str()) - Indicators::__updateTime__ >= Indicators::__fre__ || 
			atoi(time_strs[0].c_str()) != Indicators::__updateH__ 
			)
		{
			///起始时间不对
			if (atoi(time_strs[1].c_str()) % Indicators::__fre__ != 0)
			{
				int utime = atoi(time_strs[1].c_str()) - (atoi(time_strs[1].c_str()) % Indicators::__fre__);
				string sign = ":";
				string udatetime = time_strs[0].c_str() + sign + to_string(utime) + sign + time_strs[2].c_str();
				bar_time = udatetime;
				time_strs = Split(bar_time, ":");
			}
			//当前tick是否属于当前bar时间段
			newMinitue = true;
			Indicators::__newBar__ = Indicators::__bar__;
			Indicators::__barclose__ = true;
		}
	}

	if (newMinitue)
	{
		////新建bar
		Indicators::__bar__.EMPTY = false;

		strcpy(Indicators::__bar__.InstrumentID, tick.InstrumentID);
		strcpy(Indicators::__bar__.TradingDay, tick.TradingDay);
		Indicators::__bar__.Close = tick.LastPrice;
		Indicators::__bar__.High = tick.LastPrice;
		Indicators::__bar__.Low = tick.LastPrice;
		Indicators::__bar__.Volume = tick.Volume;
		Indicators::__bar__.Open = tick.LastPrice;
		strcpy(Indicators::__bar__.UpdateTime, tick.UpdateTime);
		Indicators::__updateTime__ = atoi(time_strs[1].c_str());
		Indicators::__updateH__ = atoi(time_strs[0].c_str());

		Indicators::__bar__.Turnover = tick.Turnover;
		Indicators::__bar__.PreClosePrice = tick.PreClosePrice;
		Indicators::__bar__.PreOpenInterest = tick.PreOpenInterest;
	}
	else
	{
		////更新bar
		Indicators::__bar__.Close = tick.LastPrice;
		Indicators::__bar__.High = max(Indicators::__bar__.High, tick.LastPrice);
		Indicators::__bar__.Low = min(Indicators::__bar__.Low, tick.LastPrice);

		Indicators::__bar__.Volume += tick.Volume;
		Indicators::__bar__.Turnover += tick.Turnover;
	}
}

BarInfomation* Indicators::bar()
{
	return &(Indicators::__newBar__);
}

bool Indicators::barclose()
{
	return Indicators::__barclose__;
}

void Indicators::fre(int f)
{
	Indicators::__fre__ = f;
}

///
////
double Indicators::average(double* arr, int size)
{
	double sum = 0;
	for (size_t i = 0; i < size; i++)
	{
		sum += arr[i];
	}
	return sum / size;
}

double Indicators::std(double* arr, int size)
{
	double average = Indicators::average(arr, size);
	double sum = 0;
	for (size_t i = 0; i < size; i++)
	{
		sum += pow(arr[i] - average, 2);
	}
	return sqrt(sum / size);
}

double Indicators::RSI(double close, int peridos)
{
	double profit_bar = close - Indicators::__rsi_close_pre;
	Indicators::__rsi_close_pre = close;

	if (Indicators::__rsi_count_num <= peridos)
	{
		if (__rsi_count_num == 0)
		{
			__rsi_count_num++;
			return -1;
		}
		if (profit_bar > 0)
		{
			Indicators::__rsi_proitive_mena = Indicators::__rsi_proitive_mena + profit_bar;
		}
		else
		{
			Indicators::__rsi_negative_mena = Indicators::__rsi_negative_mena - profit_bar;
		}

		if (Indicators::__rsi_count_num == peridos)
		{
			Indicators::__rsi_proitive_mena = Indicators::__rsi_proitive_mena / peridos;
			Indicators::__rsi_negative_mena = Indicators::__rsi_negative_mena / peridos;
		}

		Indicators::__rsi_count_num++;
		return -1;
	}

	if (profit_bar > 0)
	{
		Indicators::__rsi_proitive_mena = (Indicators::__rsi_proitive_mena * (peridos - 1) + profit_bar) / peridos;
		Indicators::__rsi_negative_mena = (Indicators::__rsi_negative_mena * (peridos - 1) + 0) / peridos;
	}
	else
	{
		Indicators::__rsi_proitive_mena = (Indicators::__rsi_proitive_mena * (peridos - 1) + 0) / peridos;
		Indicators::__rsi_negative_mena = (Indicators::__rsi_negative_mena * (peridos - 1) - profit_bar) / peridos;
	}
	double rs = Indicators::__rsi_proitive_mena / (Indicators::__rsi_negative_mena);

	return rs / (1 + rs) * 100;
}


double Indicators::ATR(double high, double low, double close, int timeperiod)
{
	double high_low = abs(high - low);
	double high_close = abs(high - __atr_close_pre);
	double low_close = abs(low - __atr_close_pre);
	double _arr[3] = { high_low, high_close, low_close };
	double __tr = Indicators::max_arr(_arr, 3);
	__atr_close_pre = close;

	if (__atr_count_num <= timeperiod)
	{
		if (__atr_count_num == 0)
		{
			__atr_count_num++;
			return -1;
		}
		__atr_atr += __tr;
		if (__atr_count_num == timeperiod)
		{
			__atr_atr /= timeperiod;
		}
		__atr_count_num++;
		return __atr_atr;
	}
	__atr_atr = (__atr_atr * (timeperiod - 1) + __tr) / timeperiod;
	return __atr_atr;
}

double Indicators::max_arr(double* arr, int size)
{
	double max_value = 0;
	for (size_t i = 0; i < size; i++)
	{
		if (arr[i] > max_value)
		{
			max_value = arr[i];
		}
	}
	return max_value;
}

double Indicators::ATR_compire_AATR(double atr, int timeperiod = 300)
{
	//if (__aatr_count_num <= timeperiod)
	//{
	//	__aatr_aatr += atr;
	//	
	//	if (__aatr_count_num == timeperiod)
	//	{
	//		__aatr_aatr /= timeperiod;
	//		__aatr_count_num++;
	//		return __aatr_aatr;
	//	}
	//	__aatr_count_num++;
	//	return -1;
	//}

	//__aatr_aatr = (__aatr_aatr * (timeperiod - 1) + atr) / timeperiod;

	__aatr_atr_vec.push_back(atr);
	__aatr_atr_vec.erase(__aatr_atr_vec.begin());
	if (__aatr_count_num < timeperiod)
	{
		__aatr_sum += atr;
		__aatr_count_num++;
		return -1;
	}
	__aatr_sum += atr;
	__aatr_aatr = __aatr_sum / timeperiod;
	__aatr_sum -= __aatr_atr_vec.at(0);

	return atr - __aatr_aatr;
}