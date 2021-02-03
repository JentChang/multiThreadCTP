#pragma once
#include <vector>


class MEvent
{
public:
	MEvent();
	~MEvent();

	int addnum(int);
	void subnum();
	int num();

private:
	std::vector<int> __num;
};