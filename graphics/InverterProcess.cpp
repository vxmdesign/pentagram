/*
 *  Copyright (C) 2004  Pentagram Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "pent_include.h"

#include "InverterProcess.h"
#include "Kernel.h"
#include "GUIApp.h"

#include "IDataSource.h"
#include "ODataSource.h"

unsigned int states[] = { 0, 8, 63, 211, 493, 945, 1594, 2459,
						  3552, 4870, 6406, 8139, 10042, 12078, 14207, 16384,
						  18561, 20690, 22726, 24629, 26362, 27898, 29216,
						  30308, 31174, 31823, 32274, 32556, 32704, 32760,
						  32768,
						  32775, 32831, 32979, 33261, 33713, 34362, 35227,
						  36320, 37638, 39174, 40907, 42810, 44846, 46975,
						  49152, 51328, 53457, 55494, 57396, 59129, 60665,
						  61984, 63076, 63942, 64591, 65042, 65324, 65472,
						  65528, 65536 };

InverterProcess *InverterProcess::inverter = 0;

// p_dynamic_class stuff 
DEFINE_RUNTIME_CLASSTYPE_CODE(InverterProcess,Process)

InverterProcess::InverterProcess()
	: Process()
{

}

InverterProcess::InverterProcess(unsigned int target)
	: Process()
{
	targetstate = target;
}

InverterProcess::~InverterProcess(void)
{
	if (inverter == this)
		inverter = 0;
}

bool InverterProcess::run(const uint32)
{
	GUIApp* app = GUIApp::get_instance();

	unsigned int state = app->getInversion();
	if (state == targetstate) {
		terminate();
	} else {
		unsigned int i = 0;
		while (states[i] <= state) i++;
		app->setInversion(states[i]);
	}

	return false;
}

void InverterProcess::saveData(ODataSource* ods)
{
	ods->write2(1); //version
	Process::saveData(ods);
	
	ods->write2(static_cast<uint16>(targetstate));
}

bool InverterProcess::loadData(IDataSource* ids)
{
	uint16 version = ids->read2();
	if (version != 1) return false;
	if (!Process::loadData(ids)) return false;
	
	targetstate = ids->read2();

	inverter = this; //static
	return true;
}

// static
ProcId InverterProcess::invertScreen()
{
	if (inverter) {
		if (inverter->targetstate == 0)
			inverter->setTarget(0x8000);
		else
			inverter->setTarget(0);
		return inverter->getPid();
	} else {
		unsigned int target = 0x8000;
		if (GUIApp::get_instance()->isInverted()) target = 0;
		inverter = new InverterProcess(target);
		return Kernel::get_instance()->addProcess(inverter);
	}
}


uint32 InverterProcess::I_invertScreen(const uint8* args,
									   unsigned int /*argsize*/)
{
	return invertScreen();
}

void InverterProcess::ConCmd_invertScreen(const Console::ArgsType &args, const Console::ArgvType &argv)
{
	invertScreen();
}
