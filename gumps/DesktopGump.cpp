/*
 *  Copyright (C) 2003-2004  The Pentagram Team
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
#include "DesktopGump.h"
#include "RenderSurface.h"
#include "IDataSource.h"
#include "ODataSource.h"

DEFINE_RUNTIME_CLASSTYPE_CODE(DesktopGump,Gump);

DesktopGump::DesktopGump()
	: Gump()
{

}

DesktopGump::DesktopGump(sint32 _x, sint32 _y, sint32 _width, sint32 _height) :
	Gump(_x, _y, _width, _height, 0, 0, LAYER_DESKTOP)
{
}

DesktopGump::~DesktopGump(void)
{
}

void DesktopGump::PaintThis(RenderSurface *surf, sint32 lerp_factor)
{
	// Just fill it
	surf->Fill32(0x3f3f3f, 0, 0, dims.w, dims.h);
}

bool DesktopGump::StartDraggingChild(Gump* gump, int mx, int my)
{
	gump->ParentToGump(mx, my);
	gump->SetMoveOffset(mx, my);
	return true;
}

void DesktopGump::DraggingChild(Gump* gump, int mx, int my)
{
	gump->Move(mx, my);
}

void DesktopGump::StopDraggingChild(Gump* gump)
{
	gump->SetMoveOffset(0, 0);
}

void DesktopGump::saveData(ODataSource* ods)
{
	ods->write2(1); //version
	Gump::saveData(ods);
}

bool DesktopGump::loadData(IDataSource* ids)
{
	uint16 version = ids->read2();
	if (version != 1) return false;
	if (!Gump::loadData(ids)) return false;

	return true;
}

// Colourless Protection
