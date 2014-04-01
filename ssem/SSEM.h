/***
	SSEM "Manchester Baby" Simulator
	Author:	Todd Harding
	Date: 	16/11/2012
*/
#ifndef __SSEM
#define __SSEM
#include "datatypes.h"
#include "cpu.h"
#include "mainstore.h"
#include "clock.h"
#include "VDU.h"
#include "tickable.h"

namespace mb
{
	/**
		Main SSEM class
		Holds instances of the CPU, VDU and Clock
	*/
	class SSEM implements(Tickable)
	{
	public:
		SSEM();
		~SSEM();
		
		/**
			Power On SSEM and start clock
		*/
		void powerOn();

		/**
			power off SSEM and stop clock
		*/
		void powerOff();

		/**
			Cycle SSEM
		*/
		void cycle();

		/**
			Set the step through execution mode
			@param step mode
		*/
		void setStepMode(bool mode);

		/**
			Load Binary Program
			@param program
		*/
		void loadProgram(std::vector<DWORD> program);

		/**
			Send tick to CPU and update VDU
		*/
		virtual void tick();
	private:
		bool running;
		bool m_stepMode;
		Clock m_clock;
		CPU m_cpu;
		MainStore m_store;
		VDU m_vdu;
	};
}
#endif