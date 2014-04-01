/***
  SSEM "Manchester Baby" Simulator
  Author: Todd Harding
  Date:   16/11/2012
*/
#ifndef __DATATYPES
#define __DATATYPES

#include <stdint.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <cstdlib>
#include <ctype.h>
#include <memory>
#include <exception>


typedef int8_t BYTE;
typedef int16_t WORD;
typedef int32_t DWORD;

typedef DWORD instruction;

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
  Linux clear Screen 
*/
inline 	void clear()
{
	std::cout << "\033[2J\033[1;1H";
}



#ifdef WIN32
#include <conio.h>
#elif __linux__
/**
    Linux implementation of getch
    Code Snippet From:
    http://stackoverflow.com/questions/7469139/what-is-equivalent-to-getch-getche-in-linux
  **/
#include <termios.h>
#include <stdio.h>
extern "C"
{
    static struct termios old, newn;

    /* Initialize new terminal i/o settings */
    inline void initTermios(int echo)
    {
      tcgetattr(0, &old); /* grab old terminal i/o settings */
      newn = old; /* make new settings same as old settings */
      newn.c_lflag &= ~ICANON; /* disable buffered i/o */
      newn.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
      tcsetattr(0, TCSANOW, &newn); /* use these new terminal i/o settings now */
    }

    /* Restore old terminal i/o settings */
    inline void resetTermios(void)
    {
      tcsetattr(0, TCSANOW, &old);
    }

    /* Read 1 character - echo defines echo mode */
    inline char getch_(int echo)
    {
      char ch;
      initTermios(echo);
      ch = getchar();
      resetTermios();
      return ch;
    }

    /* Read 1 character without echo */
    inline char getch(void)
    {
      return getch_(0);
    }

    /* Read 1 character with echo */
    inline char getche(void)
    {
      return getch_(1);
    }
}
// END CODE SNIPPET
#else
/**
    Fake fetch for compatibility
*/
char getch()
{
    char x;
    std::cin >> x;
    return x;
}
#endif


#define implements(X) : private X
#endif