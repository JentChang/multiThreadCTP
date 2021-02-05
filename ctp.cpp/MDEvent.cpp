#include "MDEvent.h"


vector<TickInfomation> MDEvent::__tick_vtr;
vector<StrategyTemplate*> MDEvent::strategy_vtr;
mutex MDEvent::mtx;

MDEvent::MDEvent()
{
	this->__tick_vtr.clear();
	this->strategy_vtr.clear();
	this->thread_vtr.clear();
}

MDEvent::~MDEvent()
{
	//for (size_t i = 0; i < strategy_vtr.size(); i++)
	//{
	//	this->strategy_vtr.at[i] = nullptr;
	//}
	this->strategy_vtr.clear();
	this->thread_vtr.clear();
}

void MDEvent::InsNum(int ins_num)
{
	this->__tick_vtr.resize(ins_num+2);
}

int MDEvent::AddTick(TickInfomation tick)
{
	TickInfomation tick_info = tick;

	MDEvent::mtx.lock();
	this->__tick_vtr.push_back(tick_info);
	this->__tick_vtr.erase(this->__tick_vtr.begin());
	MDEvent::mtx.unlock();

	return 0;
}


void MDEvent::AddStrategy(StrategyTemplate *st)
{
	this->strategy_vtr.push_back(st);
}

void MDEvent::StrategyStart()
{
	for (size_t i = 0; i < this->strategy_vtr.size(); i++)
	{
		///策略启动
		StrategyTemplate* st = MDEvent::strategy_vtr[i];
		st->InIt();
		///策略线程
		this->thread_vtr.push_back(thread(MDEvent::SendTickThreadFun, st));
	}
}

void MDEvent::join()
{
	for (auto& thread : this->thread_vtr) {
		thread.join();
	}
}

void MDEvent::SendTickThreadFun(StrategyTemplate* st)
{
	while (true)
	{
		MDEvent::mtx.lock();
		for (size_t i = 0; i < MDEvent::__tick_vtr.size(); i++)
		{
			TickInfomation tick = MDEvent::__tick_vtr.at(i);
			if (!tick.EMPTY &&
				strcmp(tick.datetime, st->GetTick().datetime) > 0 &&
				strcmp(tick.InstrumentID, st->InstrumentID()) == 0
				)
			{
				st->ReceiveTick(tick);
			}
		}
		MDEvent::mtx.unlock();
	}
	
}
