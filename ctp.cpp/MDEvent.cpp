#include "MDEvent.h"

map<string, TickInfomation> MDEvent::__tick_map;

bool MDEvent::ISWRITING;
int MDEvent::ISREADING;
vector<StrategyTemplate*> MDEvent::strategy_vtr;


MDEvent::MDEvent()
{
	MDEvent::ISWRITING = false;
	MDEvent::ISREADING = false;

	this->strategy_vtr.clear();
	this->thread_vtr.clear();
}

MDEvent::~MDEvent()
{
	this->strategy_vtr.clear();
	this->thread_vtr.clear();
}


int MDEvent::AddTick(TickInfomation tick)
{
	while (MDEvent::ISREADING == 0)
	{
		///�ȴ����̸߳��ƽ���
	}
	MDEvent::ISWRITING = true;
	MDEvent::__tick_map[tick.InstrumentID] = tick;
	MDEvent::ISWRITING = false;

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
		///��������
		StrategyTemplate* st = MDEvent::strategy_vtr[i];
		st->InIt();
		///�����߳�
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
	static TickInfomation tick;
	static TThostFtdcInstrumentIDType thisthread_ins;
	strcpy(thisthread_ins, st->InstrumentID());

	while (true)
	{
		while (MDEvent::ISWRITING)
		{
			///�ȴ�д�߳�д����
		}
		MDEvent::ISREADING ++;
		tick = MDEvent::__tick_map[thisthread_ins];
		MDEvent::ISREADING --;

		if (!tick.EMPTY &&
			strcmp(tick.datetime, st->GetTick().datetime) > 0 &&
			strcmp(tick.InstrumentID, st->InstrumentID()) == 0
			)
		{
			st->ReceiveTick(tick);
		}

	}
}
