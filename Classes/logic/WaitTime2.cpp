#include "TcpLogic.h"

WaitTime2::WaitTime2()
{
	state = false;
}

void WaitTime2::StartWait(int time)
{
	state = true;
	recordTime = time;
}

bool WaitTime2::OnTime(int time)
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
