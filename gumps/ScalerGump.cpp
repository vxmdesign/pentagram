/*
 *  Copyright (C) 2004  The Pentagram Team
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
#include "ScalerGump.h"

#include "RenderSurface.h"
#include "Texture.h"
#include "GUIApp.h"

DEFINE_RUNTIME_CLASSTYPE_CODE(ScalerGump,DesktopGump);

ScalerGump::ScalerGump(sint32 _x, sint32 _y, sint32 _width, sint32 _height, 
						sint32 _swidth1, sint32 _sheight1, sint32 _scaler1, 
						sint32 _swidth2, sint32 _sheight2, sint32 _scaler2)
	: DesktopGump(_x, _y, _width, _height), 
		swidth1(_swidth1), sheight1(_sheight1), scaler1(_scaler1), buffer1(0),
		swidth2(_swidth2), sheight2(_sheight2), scaler2(_scaler2), buffer2(0)
{
}

ScalerGump::~ScalerGump()
{
	delete buffer1;
	delete buffer2;
}

void ScalerGump::Paint(RenderSurface* surf, sint32 lerp_factor)
{
	// Skip the clipping rect/origin setting, since they will already be set
	// correctly by our parent.
	// (Or maybe I'm just to lazy to figure out the correct coordinates
	//  to use to compensate for the flipping... -wjp :-) )

	// Don't paint if hidden
	if (IsHidden()) return;

	int width = dims.w, height = dims.h;

	// We don't care, we are not going to support filters, at least not at the moment
	if (swidth1 == width && sheight1 == height)
	{
		PaintChildren(surf, lerp_factor);
		return;
	}

	// need a backbuffer
	if (!buffer1) {
		buffer1 = RenderSurface::CreateSecondaryRenderSurface(swidth1, sheight1);
	}

	// Paint children
	PaintChildren(buffer1, lerp_factor);

	Texture* tex = buffer1->GetSurfaceAsTexture();

	/*
	if (height==480 && sheight ==200)
	{
		surf->StretchBlit(tex, 0, 0, swidth, 1, 0, 0, width, 2, scaler1==1);

		int i = 1;
		int j = 0;
		int f = 3;
		while(i<468)
		{
			surf->StretchBlit(tex, 0, j, swidth, 3, 0, i, width, 6, scaler1==1);
			i+=5;
			j+=2;

			surf->StretchBlit(tex, 0, j, swidth, 4, 0, i, width, 8, scaler1==1);
			i+=7;
			j+=3;
		}
		surf->StretchBlit(tex, 0, j, swidth, 3, 0, i, width, 6, scaler1==1);
		i+=5;
		j+=2;

		surf->StretchBlit(tex, 0, j, swidth, 3, 0, i, width, 6, scaler1==1);
	}
	else
	*/
	{
		surf->StretchBlit(tex, 0, 0, swidth1, sheight1, 0, 0, width, height, scaler1==1);
	}

}

// Convert a parent relative point to a gump point
void ScalerGump::ParentToGump(int &px, int &py)
{
	px -= x;
	px *= swidth1;
	px /= dims.w;

	py -= y;
	py *= sheight1;
	py /= dims.h;
}

// Convert a gump point to parent relative point
void ScalerGump::GumpToParent(int &gx, int &gy)
{
	gx *= dims.w;
	gx /= swidth1;
	gx += x;

	gy *= dims.h;
	gy /= sheight1;
	gy += y;
}