/**
	SSEM Assembler
	Todd Harding
	AC21009
	23/11/12
*/
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>
#include <exception>
#include <stdio.h>

using namespace std;

const std::string punc =",.!\"/£$%^&*()_+=?><@#~':;|{}[]/ ";

typedef int8_t BYTE;
typedef int16_t WORD;
typedef int32_t DWORD;

typedef DWORD instruction;

const DWORD OpCodeOffset = 13;

/**
    checks if a string contains punctuation
    @param string s
    @return bool true if string contains punctuation
*/
bool containsPunctuation(string s)
{
    for (string::iterator iter = s.begin(); iter != s.end(); ++iter)
    {
        for (int y = 0; y < punc.size(); ++y)
        {
            if(*iter == punc.at(y))
            {
                return true;
            }
        }
    }
    return false;
}

/**
    remove punctuation from a string
    @param string s
    @return a de punctuated string
*/
string removePunctuation(string s)
{
    while(s.empty() == false && containsPunctuation(s))
    {
        for (int i = 0; i < punc.length(); ++i)
        {
            for (std::string::iterator iter = s.begin(); iter != s.end();)
            {
                if(*iter == punc.at(i))
                {
                    iter = s.erase(iter);
                }
                else
                    ++iter;
            }
        }
    }
    return s;
}

/**
  Convert Data
*/
template<typename T>
bool convertWTest(std::string const& s, T& x,
                   bool failIfLeftoverChars = true)
{
   std::istringstream i(s);
   char c;
   if (!(i >> x) || (failIfLeftoverChars && i.get(c)))
  	return false;
   else
  	return true;
}

/**
  Convert String to DWORD
  @param string to convert
  @return DWORD 
*/
inline DWORD stringBinaryToDWORD(std::string s)
{
	DWORD d = 0;
	std::reverse(s.begin(), s.end());

	for (int i = 0, c = 0; c < sizeof(DWORD)*8 && i < s.size(); ++i)
	{
		if(s.at(i) == '1')
		{
			d+= (1 << i);
			c++;
		}
		else
		if(s.at(i) == '0')
		{
			d+= (0 << i);
			c++;
		}
	}
	return d;
}

/**
  Convert DWORD to Binary String
  @param DWORD to convert
  @return string
*/
inline std::string DWORDtoBinaryString(DWORD d)
{
	std::string s;
	for (int i = 0; i < sizeof(DWORD)*8; ++i)
	{

		bool b = (d & (1 << i));

		if(b)
			s.push_back('1');
		else
			s.push_back('0');

	}

	std::reverse(s.begin(), s.end());
	
	return s;
}

/**
  reverse bits, SSEM has reversed binary
  so hypothetically it is little-endian
  not big endian
  @param bits to reverse 
*/
inline DWORD reverseBits(DWORD d)
{
	std::string s = DWORDtoBinaryString(d);
	std::reverse(s.begin(), s.end());
	return stringBinaryToDWORD(s);
}

/**
	Check if a string is a number
	@param string to check
	@return bool
*/
bool isStringANumber(string s)
{
	static string nums = "-0123456789";
	for (std::string::iterator i = s.begin(); i != s.end(); ++i)
	{
		bool isNumber = false;
		for (std::string::iterator n = nums.begin(); n != nums.end(); ++n)
		{
			if(*i == *n)
			{
				isNumber = true;
				n = nums.end()-1;
			}
		}
		if (isNumber == false)
		{
			return false;
		}
	}
	return true;
}
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
						STP, 	// Light "Stop" neon and halt the machine
						VAR, 	// Macro for variables, is a pseudo instruction
						NUM, 	// Alias for the above
						INVALID
					};

/**
	save data file to binary.ssem
	@param string data 
	@return bool true on success
*/
bool saveFile(string data)
{
	ofstream out("binary.ssem");

	if(out.good())
		out << data;
	else
	{
		out.close();
		return false;
	}

	out.close();
	return true;
}

/**
	Load File
	@param filename
	@return loaded file
*/
string loadFile(char const *filename)
{
	ifstream in(filename);
	char buffer[256];
	memset(buffer, 0, 256);

	/*	if file is bad, throw exception	*/
	if(!in.is_open() || in.bad())
		__throw_invalid_argument("Unable to open file, or file is bad");

	string file;
	while(in.good())
	{
		in.getline(buffer, 255);
		file.append(buffer);
	}
	return file;
}

/**
	Load File Into a Vector, line by line
	@param file name
	@return vector of strings
*/
vector<string> loadFileIntoVec(char const *filename)
{
	ifstream in(filename);
	char buffer[1024];
	memset(buffer, 0, 1024);

	/*	if file is bad, throw exception	*/
	if(!in.is_open() || in.bad())
		__throw_invalid_argument("Unable to open file, or file is bad");

	vector<string> file;
	while(in.good())
	{
		in.getline(buffer, 1023);
		file.push_back(buffer);
	}
	return file;
}

/**
	pre process string, remove whitespace and punctuation
	@param data string
	@return processed string
*/
string preProcess(string data)
{
	string processedData;

	unsigned int whitespacecounter = 0;
	for (std::string::iterator i = data.begin(); i != data.end(); ++i)
	{
		switch(*i)
		{
			case ' ':
			{
				if(whitespacecounter == 0 && i != data.begin())
				{
					processedData.push_back(*i);
					++whitespacecounter;
				}
				else do
				{
					++i;
				} while (i != data.end() && *i+1 == ' ' );
				whitespacecounter = 0;
			}
			break;
			case ';':
			{
				return processedData;
			}
			break;
			default:
			{
				processedData.push_back(*i);
			}
			break;
		}
	}
	return processedData;
}

/**
	Check if the string is an opcode
	@param a map of instructions
	@param the string to check
	@return a CPUinstruction, INVALID if not opcode
*/
CPUinstruction checkIfOpcode(map<string, CPUinstruction> instructionMap, string s)
{
	map<string, CPUinstruction>::iterator it;
	it = instructionMap.find(s);
	if (it != instructionMap.end())
	{
		return (*it).second;
	}
	else
		return INVALID;
}

/**
	Assemble the processed data into a string binary representation
	of SSEM machine code
	@param map of CPU instructions
	@param the symbol table
	@param the code buffer
	@param the pre processed data
	@return string containing 1's and 0's representing SSEM machine code
*/
string assemble(map<string, CPUinstruction> instructionMap, std::map<string, std::pair<bool, DWORD> >& symbolTable, std::vector<DWORD>& codeBuffer, vector<string> data )
{
	codeBuffer.resize(32);
	string output ="";
	if(data.size() > 32)
	{
		return string("Error: file over 32 lines long");
	}
	vector<vector<string> > lines;
	for (int i = 0; i < data.size(); ++i)
	{
		std::vector<string> words;
		string buffer ="";

		for (int x = 0; x < data.at(i).size(); ++x)
		{
			buffer.push_back(data.at(i).at(x));
			if(data.at(i).at(x) == ' ' || x == data.at(i).size() -1)
			{
				if(buffer.size() > 0)
				{
					string s = removePunctuation(buffer);
					if(s.size() > 0)
					{
						words.push_back(s);
						buffer.clear();
					}
				}
			}
	
		}
		if (words.size() > 0 && words.size() < 4)
		{
			lines.push_back(words);
		}
		else
		{
			stringstream ss;
			ss << "Error, Word size incorrect, too many words on line: " << i << endl;
			return ss.str();	
		}
	}

	for (std::vector<std::vector<string> >::iterator i = lines.begin(); i != lines.end(); ++i)
	{
		if((*i).size() == 0)
		{
			stringstream ss;
			ss << "Error no arguments on line: " << distance(lines.begin(), i) << endl;
			return ss.str();
		}
		else
		if((*i).size() > 3)
		{
			stringstream ss;
			ss << "Error too many arguments on line: " << distance(lines.begin(), i) << endl;
			return ss.str();
		}
		else
		if((*i).size() == 1)
		{
			CPUinstruction var0 = checkIfOpcode(instructionMap, (*i).at(0));
			if(var0 != INVALID)
			{
				if(var0 == NUM || var0 == VAR)
				{
					stringstream ss;
					ss << "Error single argument cannot be variable type, variables require an operand value, line: " << distance(lines.begin(), i) << endl;
					return ss.str();
				}
				else
				{
					codeBuffer.at(distance(lines.begin(), i)) = (var0 << OpCodeOffset);
				}
			}
			else
			{
				stringstream ss;
				ss << "Error single argument must be of type opcode, line: " << distance(lines.begin(), i) << endl;
				return ss.str();
			}
		}
		else
		if((*i).size() == 2)
		{
			// Opcode + Operand, or Opcode + Label, or Label + STP

			// check if var 0 is opcode
			CPUinstruction var0 = checkIfOpcode(instructionMap, (*i).at(0));
			if (var0 != INVALID)
			{
				// If var0 is a variable
				if(var0 == NUM || var0 == VAR)
				{
					//if var1 is a number
					if(isStringANumber((*i).at(1)))
					{
						DWORD t;
						convertWTest((*i).at(1), t);
						codeBuffer.at(distance(lines.begin(), i)) = t;
					}
					// else if var1 is a label, put the label in the symbol table
					else
					{
						symbolTable[(*i).at(1)] = pair<bool, DWORD>(false, 0);
					}
				}
				else
				{
					// shift opcode value into code buffer
					codeBuffer.at(distance(lines.begin(), i)) = (0 + ((DWORD) var0 << OpCodeOffset));
					//if var1 is a number
					if(isStringANumber((*i).at(1)))
					{
						DWORD t;
						convertWTest((*i).at(1), t);
						codeBuffer.at(distance(lines.begin(), i)) += t;
					}
					// else if var1 is a label, put the label in the symbol table
					else
					{
						symbolTable[(*i).at(1)] = pair<bool, DWORD>(false, 0);
					}
				}

			}
			// var0 is not an opcode
			else
			{
				//if var0 is a number
				if(isStringANumber((*i).at(0)))
				{
					stringstream ss;
					ss << "Error 1st argument cannot be a number, Line: " << distance(lines.begin(), i) << endl;
					return ss.str();
				}
				// else if var0 is a label
				else
				{   // if var1 is a number,set symbol var0 to DWORD var1
					if(isStringANumber((*i).at(1)))
					{
						DWORD val;
						convertWTest((*i).at(1), val);
						symbolTable[(*i).at(0)] = pair<bool, DWORD>(true, distance(lines.begin(), i));
					}
					else // if var1 is an instruction set symbol var 0 to DWORD value of var1 instruction 
					if(checkIfOpcode((instructionMap), (*i).at(1)) != INVALID)
					{
						symbolTable[(*i).at(0)] = pair<bool, DWORD>(true, distance(lines.begin(), i));
						if(checkIfOpcode(instructionMap, (*i).at(1)) != NUM || checkIfOpcode(instructionMap, (*i).at(1)) != VAR)
						{
							codeBuffer.at(distance(lines.begin(), i)) = (0 + (checkIfOpcode(instructionMap, (*i).at(1)) << OpCodeOffset));
						}
					}
					else
					{
						stringstream ss;
						ss << "Error Cannot ascertain second argument type, Line: " << distance(lines.begin(), i) << endl;
						return ss.str();
					}
				}
			}

		}
		if((*i).size() == 3)
		{
			// Label + Operand + variable
			if(isStringANumber((*i).at(0)))
			{
				stringstream ss;
				ss << "Error a line with 3 arguments cannot start with a number, line must start with a label, Line: " << distance(lines.begin(), i) << endl;
				return ss.str();
			}
			else
			if(checkIfOpcode(instructionMap, (*i).at(0)) != INVALID)
			{
				stringstream ss;
				ss << "Error a line with 3 arguments cannot start with an opcode, line must start with a label, Line: " << distance(lines.begin(), i) << endl;
				return ss.str();
			}
			else
			{
				if(checkIfOpcode(instructionMap, (*i).at(1)) != INVALID)
				{
					if(checkIfOpcode(instructionMap, (*i).at(1)) == NUM || checkIfOpcode(instructionMap, (*i).at(1)) == VAR)
					{
						if (isStringANumber((*i).at(2)))
						{
							DWORD val;
							convertWTest((*i).at(2), val);
							codeBuffer.at(distance(lines.begin(), i)) = val;
							symbolTable[(*i).at(0)] = pair<bool, DWORD>(true, distance(lines.begin(), i));
						}
						else
						{
							symbolTable[(*i).at(0)] = pair<bool, DWORD>(true, distance(lines.begin(), i));
							if(symbolTable[(*i).at(2)].first == false)
								symbolTable[(*i).at(2)] = pair<bool, DWORD>(false, 0);
						}
					}
					else
					if(isStringANumber((*i).at(2)))
					{
						DWORD opcode = (0 + (checkIfOpcode(instructionMap, (*i).at(1)) << OpCodeOffset));
						DWORD operand;
						convertWTest((*i).at(2), operand);
						codeBuffer.at(distance(lines.begin(), i)) = opcode + operand;
						symbolTable[(*i).at(0)] = pair<bool, DWORD>(true, distance(lines.begin(), i));
					}
					else
					{
						DWORD opcode = (0 + (checkIfOpcode(instructionMap, (*i).at(1)) << OpCodeOffset));
						codeBuffer.at(distance(lines.begin(), i)) = opcode;
						symbolTable[(*i).at(0)] = pair<bool, DWORD>(true, distance(lines.begin(), i));
						if(symbolTable[(*i).at(2)].first == false)
							symbolTable[(*i).at(2)] = pair<bool, DWORD>(false, 0);
					}
				}
				else
				{
					stringstream ss;
					ss << "Error, on a line with 3 arguments, the second argument must be an opcode, Line: " << distance(lines.begin(), i) << endl;
					return ss.str();
				}
			}
		}
	}

	for (std::map<string, pair<bool, DWORD> >::iterator i = symbolTable.begin(); i != symbolTable.end(); ++i)
	{
		if((*i).second.first == false)
		{
			stringstream ss;
			ss << "Error, Symbol  " << (*i).first << " has not been resolved" << endl;
			return ss.str();
		}
	}

	for (std::vector<vector<string> >::iterator i = lines.begin(); i != lines.end(); ++i)
	{
		map<string, pair<bool, DWORD> >::iterator mIter = symbolTable.find((*i).back());

		if(mIter != symbolTable.end())
		{
			codeBuffer.at(distance(lines.begin(), i)) += (*mIter).second.second;
		}

	}

	stringstream ss;
	for (std::vector<DWORD>::iterator i = codeBuffer.begin(); i != codeBuffer.end(); ++i)
	{
		ss << DWORDtoBinaryString(reverseBits(*i)) << endl;
	}

	return ss.str();
}


int main(int argc, char const *argv[])
{


	/*	Program only takes 1 argument, the filename	*/
	if(argc < 2 || argc > 2)
	{	
		cout << "Program only takes 1 argument, the filename of the assembly file" << endl;
		return 1;
	}

	std::map<string, std::pair<bool, DWORD> > symbolTable;

	std::vector<DWORD> codeBuffer;
	codeBuffer.resize(32);

	static std::map<string, CPUinstruction> instructionMap;

	instructionMap.insert(pair<string, CPUinstruction>("JMP", JMP));
	instructionMap.insert(pair<string, CPUinstruction>("JRP", JRP));
	instructionMap.insert(pair<string, CPUinstruction>("LDN", LDN));
	instructionMap.insert(pair<string, CPUinstruction>("STO", STO));
	instructionMap.insert(pair<string, CPUinstruction>("SUB", SUB));
	instructionMap.insert(pair<string, CPUinstruction>("SUB2", SUB2));
	instructionMap.insert(pair<string, CPUinstruction>("CMP", CMP));
	instructionMap.insert(pair<string, CPUinstruction>("STP", STP));
	instructionMap.insert(pair<string, CPUinstruction>("VAR", VAR));
	instructionMap.insert(pair<string, CPUinstruction>("NUM", NUM));
	instructionMap.insert(pair<string, CPUinstruction>("INVALID", INVALID));

	vector<string> data = loadFileIntoVec(argv[1]);
	vector<string> processedData;

	unsigned int it = 1;
	for (std::vector<string>::iterator i = data.begin(); i != data.end(); ++i)
	{
		string s = preProcess(*i);
		if(s.size() > 0)
		{
			stringstream ss;
			cout << "Line" << it << ": " << s << endl;
			processedData.push_back(s);
			++it;
		}
	}

	data.clear();

	string finaldata = assemble(instructionMap, symbolTable, codeBuffer, processedData);
	if(finaldata.find("Error") != string::npos)
	{
		cout << finaldata << endl;
		cout << "Unable to save due to error" << endl;
		return 1;
	}
	saveFile(finaldata);
	cout << endl << "binary.ssem succesfully created" << endl;
	return 0;
}
