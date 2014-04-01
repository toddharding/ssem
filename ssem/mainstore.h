/***
	SSEM "Manchester Baby" Simulator
	Author:	Todd Harding
	Date: 	16/11/2012
*/
#ifndef __MAINSTORE
#define __MAINSTORE
#include <vector> 
#include "datatypes.h"


namespace mb
{	
	/**
		Main Store (RAM) class
	*/
	class MainStore
	{
	public:
		MainStore();
		~MainStore();

		/**
			get value at address
			@param address
		*/
		DWORD 	get(DWORD address);

		/**
			set value at address
			@param address
			@param value
		*/
		void 	set(DWORD address, DWORD value );

		/**
			get the contents of memory
			@return store contents
		*/
		std::vector<DWORD>	getData();

	private:
		std::vector<DWORD> data;
	};
}
#endif