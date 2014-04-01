#include "mainstore.h"

using namespace mb;

MainStore::MainStore()
{
	data.resize(32);
	for (std::vector<DWORD>::iterator i = data.begin(); i != data.end(); ++i)
	{
		*i = 0;
	}

}

MainStore::~MainStore()
{

}

void MainStore::set(DWORD address, DWORD value )
{
	data.at(address) = value;
}

DWORD MainStore::get(DWORD address)
{
	return data.at(address);
}

std::vector<DWORD> MainStore::getData()
{
	return data;
}