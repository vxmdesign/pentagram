/*
 *  Copyright (C) 2003 The Pentagram Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "pent_include.h"

#include "AnimDat.h"

#include "IDataSource.h"
#include "ActorAnim.h"
#include "AnimAction.h"

AnimDat::AnimDat()
{

}


AnimDat::~AnimDat()
{
	for (unsigned int i = 0; i < anims.size(); i++)
		delete anims[i];
	anims.clear();
}

ActorAnim* AnimDat::getAnim(uint32 shape) const
{
	if (shape >= anims.size()) return 0;

	return anims[shape];
}

AnimAction* AnimDat::getAnim(uint32 shape, uint32 action) const
{
	if (shape >= anims.size()) return 0;
	if (anims[shape] == 0) return 0;

	return anims[shape]->getAction(action);
}


void AnimDat::load(IDataSource *ds)
{
	AnimFrame f;

	// CONSTANT !
	anims.resize(2048);

	for (unsigned int shape = 0; shape < anims.size(); shape++)
	{
		ds->seek(4*shape);
		uint32 offset = ds->read4();

		if (offset == 0) {
			anims[shape] = 0;
			continue;
		}

		ActorAnim *a = new ActorAnim();

		// CONSTANT !
		a->actions.resize(64);

		for (unsigned int action = 0; action < a->actions.size(); action++)
		{
			ds->seek(offset + action*4);
			uint32 actionoffset = ds->read4();

			if (actionoffset == 0) {
				a->actions[action] = 0;
				continue;
			}

			a->actions[action] = new AnimAction();

			ds->seek(actionoffset);
			uint32 actionsize = ds->read1();
			a->actions[action]->size = actionsize;
			ds->skip(1); // unknown
			a->actions[action]->framerepeat = ds->read1();
			ds->skip(1); // unknown

			for (unsigned int dir = 0; dir < 8; dir++)
			{
				a->actions[action]->frames[dir].clear();

				for (unsigned int j = 0; j < actionsize; j++)
				{
					f.frame = ds->read1(); // & 0x7FF;
					uint8 x = ds->read1();
					f.frame += (x & 0x7) << 8;
					f.deltaz = ds->readXS(1);
					f.sfx = ds->read1();
					f.deltadir = ds->readXS(1);
					f.flags = ds->read1();
					f.flags += (x & 0xF8) << 8;
					a->actions[action]->frames[dir].push_back(f);
				}
			}
		}

		anims[shape] = a;
	}
}
