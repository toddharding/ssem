#include "VDU.h"

using namespace mb;

VDU::VDU() : m_frameSkip(0), m_currentFrame(0)
{

}

VDU::~VDU()
{
	
}

void VDU::update(std::vector<DWORD> binary)
{
	clear();
	for (std::vector<DWORD>::iterator i = binary.begin(); i != binary.end(); ++i)
	{	
		std::cout << convertToPulses(DWORDtoBinaryString(reverseBits(*i))) << std::endl;
	}
}

void VDU::update(std::vector<DWORD> binary, DWORD accumulator, DWORD controlInstruction, instruction presentInstruction)
{

	if(m_frameSkip <= 0 || (m_currentFrame % m_frameSkip == 0))
	{
		update(binary);

		std::cout << std::endl << std::endl;
		std::cout << convertToPulses(DWORDtoBinaryString(reverseBits(accumulator))) << " Accumulator" << std::endl;
		std::cout << convertToPulses(DWORDtoBinaryString(reverseBits(controlInstruction))) << " CI Control Instruction" << std::endl;
		std::cout << convertToPulses(DWORDtoBinaryString(reverseBits(presentInstruction))) << " PI Present Instruction" << std::endl;
		m_currentFrame = 0;
	}
	m_currentFrame++;
}

std::string VDU::convertToPulses(std::string s)
{

	std::string termPulses;
	const std::string BrightColor = "\033[1;32m";
	const std::string DimColor = "\033[0;32m";
	for(std::string::iterator sIter = s.begin(); sIter != s.end(); ++sIter)
	{
		if(*sIter == '1')
		{
			termPulses += BrightColor + "\u2022";
			//std::cout << BrightColor << "\u2022";
		}
		else
		if(*sIter == '0')
		{
			//std::cout << DimColor << "\u00B7";
			termPulses += DimColor + "\u00B7";
		}
	}

	termPulses += "\033[0m";
	//std::cout << "\033[0m";
	return termPulses;
}