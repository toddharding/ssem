/***
	SSEM "Manchester Baby" Simulator
	Author:	Todd Harding
	Date: 	16/11/2012
*/
#ifndef __CPU
#define __CPU 
#include <bitset>
#include "datatypes.h"
#include "mainstore.h"
#include "tickable.h"
#include <iostream>

namespace mb
{
	/**
		Enumerator for the phase of execution
	*/
	enum executionPhase {HALT, FETCH, DECODE, EXECUTE };
	const int numOfCPUPhases = 4;
	const int loopStartPhase = 1;

	/**
		Instruction Enumerators
	*/
	enum CPUinstruction	{
							JMP=0, 	// copy content of store line to CI
							JRP, 	// Add content of store line to CI	
							LDN,	// Copy Content of store line, negated, to accumulator
							STO,	// Copy Content of Accumulator to Store Line
							SUB,	// Subtract content of Accumulator to store line
							SUB2,	// Same as above
							CMP,	// Skip next instruction if content of accumulator is negative
							STOP 	// Light "Stop" neon and halt the machine
						};

	/**
		CPU Class
		Holds Accumulator CI and PI
	*/
	class CPU implements(Tickable)
	{
	public:
		/**
			Constructor
			@param reference to Main Store
		*/
		CPU(MainStore& store);
		~CPU();

		/**
			run phase of CPU
		*/
		virtual void tick();

		/**
			Start CPU
		*/
		void start();

		/**
			get the current execution phase of the CPU
			@return Execution Phase
		*/
		executionPhase getPhase();

		/**
			Get the value in the accumulator
			@return accumulator value
		*/
		DWORD getAccumulator();

		/**
			Get the value in the control instruction
			@return CI value
		*/
		DWORD getControlInstruction();

		/**
			Get the value in the present instruction
			@return PI
		*/
		instruction getPresentInstruction();
	private:
		
		/**
			Fetch
		*/
		void fetch();

		/**
			Decode
		*/
		void decode();

		/**
			Execute
		*/
		void execute();


		executionPhase 	phase;
		CPUinstruction 	decodedInstruction;
		DWORD 		  	decodedLineNumber; 	

		MainStore& store;
		///////////////////////////
		//	Registers
		///////////////////////////
		DWORD accumulator;

		union // CI(Control Instruction) or program counter
		{
			DWORD instructionAddress; 	
			DWORD programCounter;
			DWORD controlInstruction;
		};

		union // PI(Present Instruction) or instruction decode
		{
			instruction presentInstruction;			
			instruction instructionDecode;
		};
		/* data */
	};
}
#endif