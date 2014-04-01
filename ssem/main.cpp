/***
	SSEM "Manchester Baby" Simulator
	Author:	Todd Harding
	Date: 	16/11/2012
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <exception>
#include "datatypes.h"
#include "SSEM.h"

const unsigned int numOfValidOps = 2;
const std::string validOpCodes[] = 
{
	"-f", // File usage:	 		SSEM -f Binary.txt
	"-s" // Step Mode usage:	SSEM -s on, SSEM -s off
};

/**
	Checks if the "OpCode" is valid
	@param string opcode
	@return true if valid
**/
bool validOpcode(std::string opCode)
{
	for(int i = 0; i < numOfValidOps; ++i)
	{
		if(opCode == validOpCodes[i])
			return true;
	}
	return false;
}

/**
	Converts data types
	@param string data, item reference
	@return true if operation succeeded
**/
template<typename T>
bool convert(std::string arg, T& item)
{
	std::stringstream ss;
	ss << arg;

	if (ss >> item)
	{
		return true;
	}
	else 
		return false;
}

int main(int argc, char const *argv[])
{
	//declare variables for arg storage
	std::vector<DWORD> data;
	std::string filename;
	bool stepMode = false;

	std::vector<std::string> arguments;
	std::vector<std::pair<std::string, std::string> > ops;

	std::cout << "number of args: " << argc << std::endl;

	// Iterate through args and push into arguments and ops vectors 
	try
	{
		for (int i = 0; i < argc; ++i)
		{
			arguments.push_back(argv[i]);
		}

		for(int i = 0; i < arguments.size(); ++i )
		{
			if(arguments.at(i).size() >=2 && arguments.at(i).at(0) == '-' && validOpcode(arguments.at(i)))
			{
				if(arguments.at(i+1).size() > 0 && arguments.at(i+1).at(0) != '-')
				{
					std::pair<std::string, std::string> tpair;

					tpair.first = arguments.at(i);
					tpair.second = arguments.at(i + 1);
					ops.push_back(tpair);
				}
			}
		}
	}
	catch(std::exception& e)
	{
		std::cout << std::endl << "Error while parsing arguments" << std::endl << e.what();
		return 1;
	}

	// iterate through each op and set the command line option
	for (std::vector<std::pair<std::string, std::string> >::iterator i = ops.begin(); i != ops.end(); ++i)
	{
		try
		{
			if(i->first == "-f")
			{
				convert(i->second, filename);
				std::cout << "Loading: " << filename << std::endl;
			}
			else
			if(i->first == "-s")
			{
				if(i->second == "on")
				{
					stepMode = true; 
				}
			}
	
		}
		catch (std::exception& e)
		{
			std::cout << std::endl << "Error while running arguments" << std::endl << e.what();
			return 2;
		}
	}

	// try to load file
	if (filename.size() > 0)
	{
		try
		{
			
			std::ifstream in(filename.c_str());

			if(!in)
			{
				std::cout << "cannot Open File" << std::endl;
				return 1;
			}
			
			char n;
			std::string input="";
			int i = 0;
			while(in >> n)
			{
				if(n == '1' || n == '0')
					input.push_back(n), ++i;
				
				if(i == sizeof(DWORD)*8 )
				{				
					data.push_back(reverseBits(stringBinaryToDWORD(input)));
					std::cout << input << std::endl;
					input.clear();
					n = 'f';
					i = 0;
				}
			}
			//std::cin.get();
			in.close();
		}
		catch(std::exception e)
		{
			std::cout << e.what();
		}
		
	}
	else // load in built in sample program
	{
		// generate sample program
		data.push_back(DWORD(0b00000000000000000000000000000000)); 	//1 			VAR 0       ; Declare 32-bit variable to fill space
		data.push_back(DWORD(0b11100000000000100000000000000000)); 	//2 	START:  LDN NUM01   ; Copy variable to accumulator (negated)
		data.push_back(DWORD(0b00010000000000010000000000000000)); 	//3 			SUB NUM02   ; Subtract variable from accumulator to get sum (negated)
		data.push_back(DWORD(0b10010000000001100000000000000000)); 	//4				STO MYSUM   ; Store accumulator to variable - saves answer
																	//  	;The next two instructions negate the stored answer:
		data.push_back(DWORD(0b10010000000000100000000000000000)); 	//5				LDN MYSUM   ; Copy variable to accumulator (negated)
		data.push_back(DWORD(0b10010000000001100000000000000000)); 	//6				STO MYSUM   ; Store accumulator to variable
		data.push_back(DWORD(0b00000000000001110000000000000000)); 	//7 	END: 	STP         ; Stop processor
		data.push_back(DWORD(0b10000000001000000000000000000000)); 	//8 	NUM01: 	VAR 1025    ; Declare 32-bit variable
		data.push_back(DWORD(0b10110110010000000000000000000000)); 	//9 	NUM02: 	VAR 621     ; Declare 32-bit variable
		data.push_back(DWORD(0b00000000000000000000000000000000)); 	//10 	MYSUM:	VAR 0       ; Declare 32-bit variable

		for (std::vector<DWORD>::iterator i = data.begin(); i != data.end(); ++i)
		{
			*i = reverseBits(*i);
		}
	}
	
	// Create SSEM object, set variables and start cycle
	mb::SSEM manB;
	manB.powerOn();
	manB.loadProgram(data);
	manB.setStepMode(stepMode);
	manB.cycle();
	return 0;
}




