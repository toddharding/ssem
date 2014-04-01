/***
	SSEM "Manchester Baby" Simulator
	Author:	Todd Harding
	Date: 	16/11/2012
*/
#ifndef __TICKABLE
#define __TICKABLE 

namespace mb
{
	/**
		Abstract Interface Class
	*/
	class Tickable
	{
	public:
		virtual void tick()=0;
	};
}
#endif