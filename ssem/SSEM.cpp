#include "SSEM.h"

using namespace mb;

SSEM::SSEM() : 	running(false), 
				m_clock(20000), 
				m_cpu(m_store) 
{

}

SSEM::~SSEM()
{
	if(running == true)
	{
		powerOff();
	}
}

void SSEM::powerOn()
{
	running = true;
	m_clock.addReceiver(this);
	m_clock.start();
}

void SSEM::powerOff()
{
	running = false;
	m_clock.removeReceiver(this);
	m_clock.stop();
}

void SSEM::loadProgram(std::vector<DWORD> data)
{
	for (int i = 0; i < data.size(); ++i)
	{
		m_store.set(i, data.at(i));
	}
}

void SSEM::cycle()
{
	m_cpu.start();
	while (running == true)
	{
		m_clock.sendTickIfTime();
	}
}

void SSEM::tick()
{

	m_cpu.tick();
	if(m_cpu.getPhase() == FETCH)
	{
		m_vdu.update(m_store.getData(), m_cpu.getAccumulator(), m_cpu.getControlInstruction(), m_cpu.getPresentInstruction());

		if(m_stepMode)
		{
			std::cout << std::endl <<  "press any key to step execution, ctrl+c to exit" << std::endl;
			getch();
		}
		else
		{
			std::cout << std::endl <<  "press ctrl+c to exit" << std::endl;
		}
	}
	
	if(m_cpu.getPhase() == HALT)
	{
		powerOff();
		std::cout << std::endl << "HALT!" << std::endl;
	}
}

void SSEM::setStepMode(bool mode)
{
	m_stepMode = mode;
}