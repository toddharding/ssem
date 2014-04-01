#include "cpu.h"

using namespace mb;

CPU::CPU(MainStore& m) : phase(HALT), store(m) 
{
	programCounter 				= 0b00000000000000000000000000000000;
	presentInstruction 			= 0b00000000000000000000000000000000;
	accumulator 				= 0b00000000000000000000000000000000;
}

CPU::~CPU()
{

}

void CPU::start()
{
	phase = FETCH;
}

DWORD CPU::getAccumulator()
{
	return accumulator;
}
DWORD CPU::getControlInstruction()
{
	return controlInstruction;
}
instruction CPU::getPresentInstruction()
{
	return presentInstruction;
}

void CPU::tick()
{
	switch(phase)
	{
		case HALT:
		{
			return;
		}
		break;
		case FETCH:
		{
			fetch();
			phase = DECODE;
		}
		break;
		case DECODE:
		{
			decode();
			phase = EXECUTE;
		}
		break;
		case EXECUTE:
		{
			execute();
			if(phase != HALT)
				phase = FETCH;
		}
	}
}

void CPU::fetch()
{
	controlInstruction++;
	presentInstruction = store.get(controlInstruction);
	//clear();
}

void CPU::decode()
{
	bool a(0), b(0), c(0); 
	std::bitset<sizeof(DWORD)*8> bits(reverseBits(presentInstruction));

	if (bits[18])
		a = true;
	if (bits[17])
		b = true;
	if (bits[16])
		c = true;

	DWORD x = 0;
	if(a)
		x += 	(1 << 0);
	if(b)
		x += 	(1 << 1);
	if(c)
		x += 	(1 << 2);
	decodedInstruction = (CPUinstruction)x;

	decodedLineNumber = 0;
	if(bits[31])
		decodedLineNumber += (1<<0);
	if(bits[30])
		decodedLineNumber += (1<<1);
	if(bits[29])
		decodedLineNumber += (1<<2);
	if(bits[28])
		decodedLineNumber += (1<<3);
	if(bits[27])
		decodedLineNumber += (1<<4);
	
}

void CPU::execute()
{
	CPUinstruction i = (CPUinstruction) decodedInstruction;
	switch(i)
	{
		case JMP:
		{
			controlInstruction = store.get(decodedLineNumber);
		}
		break;
		case JRP:
		{
			controlInstruction = store.get(controlInstruction + decodedLineNumber);
		}
		break;
		case LDN:
		{
			accumulator = -store.get(decodedLineNumber);
		}
		break;
		case STO:
		{
			store.set(decodedLineNumber, accumulator);
		}
		break;
		case SUB:
		{
			accumulator -= store.get(decodedLineNumber);
		}
		break;
		case SUB2:
		{
			accumulator -= store.get(decodedLineNumber);
		}
		break;
		case CMP:
		{
			if(accumulator < 0)
				++controlInstruction;
		}
		break;
		case STOP:
		{
			phase = HALT;
		}
		break;
		default:
		{
			phase = HALT;
		}
	}
}

executionPhase CPU::getPhase()
{
	return phase;
}