/***
	SSEM "Manchester Baby" Simulator
	Author:	Todd Harding
	Date: 	16/11/2012
*/
#ifndef __CLOCK
#define __CLOCK
#include <time.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include "tickable.h"
namespace mb
{	
	/**
		Clock Class
		Fires events to registered "tickable" objects
	*/
	class Clock
	{
	public:
		/**
			Clock Constructor
			@param time between ticks
		*/
		Clock(int timeBetweenTicks);
		~Clock();

		/**
			Start Clock
		*/
		void start();

		/**
			Stop Clock
		*/
		void stop();

		/**
			Add a tickable object to the clock
			@param tickable object
		*/
		void addReceiver	(Tickable* t);

		/**
			Remove a tickable object from the clock
			@param tickable object
		*/
		void removeReceiver	(Tickable* t);

		/**
			Send tick event if it is time
		*/
		void sendTickIfTime();

		/**
			loop and send tick events
		*/
		void loopBySelf();
	private:
		void tick();

		bool running;
		std::vector<Tickable*> receivers;
		int m_timeBetweenTicks;
		int m_elapsedTime;
	};
}
#endif