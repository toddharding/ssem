/***
	SSEM "Manchester Baby" Simulator
	Author:	Todd Harding
	Date: 	16/11/2012
*/
#ifndef __VDU
#define __VDU

#include <vector>
#include <string>
#include "datatypes.h"
#include "tickable.h"

namespace mb
{
	/**
		VDU Visual Display Unit Class
	*/
	class VDU 
	{
	public:
		VDU();
		~VDU();

		/**
			updates display
			@param store
		*/
		void update(std::vector<DWORD> binary);

		/**
			updates display 
			@param store
			@param accumulator
			@param control instruction
			@param present instruction
		*/
		void update(std::vector<DWORD> binary, DWORD accumulator, DWORD controlInstruction, instruction presentInstruction);

		/**
			Set Frame skip value 
			@param frames to skip
		*/
		void setFrameSkip(unsigned int fskip);

	private:
		/**
			Convert 1's and 0's to unicode pulses (1 = Bullet, 0 = Middle Dot)
		*/
		std::string convertToPulses(std::string s);
		unsigned int m_frameSkip;
		unsigned int m_currentFrame;
	};
}
#endif