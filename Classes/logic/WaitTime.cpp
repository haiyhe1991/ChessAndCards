#include "GameLogic.h"

WaitTime::WaitTime()
{
	state = false;
}

void WaitTime::StartWait(int time)
{
	state = true;
	recordTime = time;
}

bool WaitTime::OnTime(int time)
{
	if (state)
	{
		recordTime -= time;
		if (recordTime > 0)
		{
			return false;
		}
		else
		{
			state = false;
		}
	}
	return true;
}
