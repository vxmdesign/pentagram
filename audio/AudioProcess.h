/*
Copyright (C) 2005 The Pentagram team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef AUDIOPROCESS_H_INCLUDED
#define AUDIOPROCESS_H_INCLUDED

#include "Process.h"
#include <list>

class AudioProcess :
	public Process
{
	struct SampleInfo {
		sint32 sfxnum;
		sint32 priority;
		ObjId objid;
		sint32 loops;
		sint32 channel;
		
		SampleInfo() : sfxnum(-1) { }
		SampleInfo(sint32 s,sint32 p,ObjId o,sint32 l,sint32 c) : 
			sfxnum(s),priority(p),objid(o),loops(l),channel(c) { }
	};

	std::list<SampleInfo>	sample_info;

public:
	// p_dynamic_class stuff
	ENABLE_RUNTIME_CLASSTYPE();

	AudioProcess(void);
	virtual ~AudioProcess(void);

	//! Get the current instance of the Audio Processes
	static AudioProcess	* get_instance() { return the_audio_process; }

	INTRINSIC(I_playSFX);
	INTRINSIC(I_playAmbientSFX);
	INTRINSIC(I_isSFXPlaying);
	INTRINSIC(I_setVolumeSFX);
	INTRINSIC(I_stopSFX);

	virtual bool run(const uint32 framenum);

	bool loadData(IDataSource* ids, uint32 version);

	void playSFX(int sfxnum, int priority, ObjId objid, int loops);
	void stopSFX(int sfxnum, ObjId objid);
	bool isSFXPlaying(int sfxnum);

	// AudioProcess::playSound console command
	//static void ConCmd_playSound(const Console::ArgsType &args, const Console::ArgvType &argv);

private:
	virtual void saveData(ODataSource* ods);

	static AudioProcess	*	the_audio_process;
};

#endif