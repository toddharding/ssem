#include "clock.h"

using namespace mb;

Clock::Clock(int timeBetweenTicks) : m_timeBetweenTicks(timeBetweenTicks), m_elapsedTime(0)
{

}

Clock::~Clock()
{
	
}

void Clock::addReceiver(Tickable* t)
{
	receivers.push_back(t);
}

void Clock::removeReceiver(Tickable* t)
{
	std::remove(receivers.begin(), receivers.end(), t);
}

void Clock::start()
{
	m_elapsedTime = clock();
	running = true;
}

void Clock::stop()
{
	m_elapsedTime = 0;
	running = false;
}

void Clock::tick()
{
	for (std::vector<Tickable*>::iterator i = receivers.begin(); i != receivers.end(); ++i)
	{
		(*i)->tick();
	}
}

void Clock::sendTickIfTime()
{
	int t = clock();

	if(t - m_elapsedTime > m_timeBetweenTicks)
	{
		tick();
		m_elapsedTime = clock();
	}
	
}

void Clock::loopBySelf()
{
	while(receivers.size() > 0)
	{
		sendTickIfTime();
	}
}