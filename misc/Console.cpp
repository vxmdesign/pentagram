/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// console.c

#include "pent_include.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

using namespace std;

#define MAXPRINTMSG	4096

// The console
Console		con;

// Standard Output Stream Object
console_ostream<char>		pout;

// Error Output Stream Object
console_err_ostream<char>	perr;

/*
================
Con_Clear_f
================
*/
void Console::Clear ()
{
	std::memset (text, ' ', CON_TEXTSIZE);
}

						
/*
================
Con_Dump_f

Save the console contents out to a file
================
*/
void Console::Dump (const char *name)
{
	int		l, x;
	char	*line;
	FILE	*f;
	char	buffer[1024];

	Printf ("Dumped console text to %s.\n", name);
	f = std::fopen (name, "wa");
	if (!f)
	{
		Print ("ERROR: couldn't open.\n");
		return;
	}

	// skip empty lines
	for (l = current - totallines + 1 ; l <= current ; l++)
	{
		line = text + (l%totallines)*linewidth;
		for (x=0 ; x<linewidth ; x++)
			if (line[x] != ' ')
				break;
		if (x != linewidth)
			break;
	}

	// write the remaining lines
	buffer[linewidth] = 0;
	for ( ; l <= current ; l++)
	{
		line = text + (l%totallines)*linewidth;
		std::strncpy (buffer, line, linewidth);
		for (x=linewidth-1 ; x>=0 ; x--)
		{
			if (buffer[x] == ' ')
				buffer[x] = 0;
			else
				break;
		}
		for (x=0; buffer[x]; x++)
			buffer[x] &= 0x7f;

		std::fprintf (f, "%s\n", buffer);
	}

	std::fclose (f);
}
						

/*
================
Con_CheckResize

If the line width has changed, reformat the buffer.
================
*/
void Console::CheckResize (int scrwidth)
{
	int		i, j, width, oldwidth, oldtotallines, numlines, numchars;
	char	tbuf[CON_TEXTSIZE];

	width = (scrwidth >> 3) - 2;

	if (width == linewidth)
		return;

	if (width < 1)			// video hasn't been initialized yet
	{
		width = 80;
		linewidth = width;
		totallines = CON_TEXTSIZE / linewidth;
		std::memset (text, ' ', CON_TEXTSIZE);
	}
	else
	{
		oldwidth = linewidth;
		linewidth = width;
		oldtotallines = totallines;
		totallines = CON_TEXTSIZE / linewidth;
		numlines = oldtotallines;

		if (totallines < numlines)
			numlines = totallines;

		numchars = oldwidth;
	
		if (linewidth < numchars)
			numchars = linewidth;

		std::memcpy (tbuf, text, CON_TEXTSIZE);
		std::memset (text, ' ', CON_TEXTSIZE);

		for (i=0 ; i<numlines ; i++)
		{
			for (j=0 ; j<numchars ; j++)
			{
				text[(totallines - 1 - i) * linewidth + j] =
						tbuf[((current - i + oldtotallines) %
							  oldtotallines) * oldwidth + j];
			}
		}
	}

	current = totallines - 1;
	display = current;
}

// Constructor
Console::Console ()
{
	linewidth = -1;

	CheckResize (0);
	
	PrintInternal ("Console initialized.\n");
}

// Destructor
Console::~Console()
{
}


//
// Internal Methods
//

// Print a text string to the console
void Console::PrintInternal (const char *txt)
{
	int		y;
	int		c, l;
	static int	cr;

	while ( (c = *txt) )
	{
	// count word length
		for (l=0 ; l< linewidth ; l++)
			if ( txt[l] <= ' ')
				break;

	// word wrap
		if (l != linewidth && (x + l > linewidth) )
			x = 0;

		txt++;

		if (cr)
		{
			current--;
			cr = false;
		}
		
		if (!x) Linefeed ();

		switch (c)
		{
		case '\n':
			x = 0;
			break;

		case '\r':
			x = 0;
			cr = 1;
			break;

		default:	// display character and advance
			y = current % totallines;
			text[y*linewidth+x] = c;
			x++;
			if (x >= linewidth) x = 0;
			break;
		}
		
	}
}

// Add a linefeed to the buffer
void Console::Linefeed (void)
{
	x = 0;
	if (display == current) display++;
	current++;
	std::memset (&text[(current%totallines)*linewidth], ' ', linewidth);
}


//
// STDOUT Methods
//

// Print a text string to the console, and output to stdout
void Console::Print (const char *txt)
{
	fputs(txt, stdout);
	PrintInternal (txt);
}

// printf, and output to stdout
int Console::Printf (const char *fmt, ...)
{
	va_list		argptr;
	char		msg[MAXPRINTMSG];

	va_start (argptr,fmt);
	vfprintf (stdout, fmt,argptr);
	int count = vsnprintf (msg,MAXPRINTMSG,fmt,argptr);
	va_end (argptr);

	PrintInternal (msg);

	return count;
}

// putchar, and output to stdout
void Console::Putchar (int c)
{
	char t[2] = {c,0};
	fputc(c, stdout);
	PrintInternal(t);
}


//
// STDERR Methods
//

// Print a text string to the console, and output to stdout
void Console::Print_err (const char *txt)
{
	fputs(txt, stderr);
	PrintInternal (txt);
}

// printf, and output to stdout
int Console::Printf_err (const char *fmt, ...)
{
	va_list		argptr;
	char		msg[MAXPRINTMSG];

	va_start (argptr,fmt);
	int count = vsnprintf (msg,MAXPRINTMSG,fmt,argptr);
	vfprintf (stderr, fmt,argptr);
	va_end (argptr);

	PrintInternal (msg);

	return count;
}

// putchar, and output to stdout
void Console::Putchar_err (int c)
{
	char t[2] = {c,0};
	fputc(c, stderr);
	PrintInternal(t);
}

/*
==============================================================================

DRAWING

==============================================================================
*/


/*
================
Con_DrawConsole

Draws the console with the solid background
================
*/
#if 0
void Con_DrawConsole (float frac)
{
	int				i, j, x, y, n;
	int				rows;
	char			*text;
	int				row;
	int				lines;
	char			version[64];
	char			dlbar[1024];

	lines = viddef.height * frac;
	if (lines <= 0)
		return;

	if (lines > viddef.height)
		lines = viddef.height;

// draw the background
	re.DrawStretchPic (0, -viddef.height+lines, viddef.width, viddef.height, "conback");
	SCR_AddDirtyPoint (0,0);
	SCR_AddDirtyPoint (viddef.width-1,lines-1);

	Com_sprintf (version, sizeof(version), "v%4.2f", VERSION);
	for (x=0 ; x<5 ; x++)
		re.DrawChar (viddef.width-44+x*8, lines-12, 128 + version[x] );

// draw the text
	vislines = lines;
	
#if 0
	rows = (lines-8)>>3;		// rows of text to draw

	y = lines - 24;
#else
	rows = (lines-22)>>3;		// rows of text to draw

	y = lines - 30;
#endif

// draw from the bottom up
	if (display != current)
	{
	// draw arrows to show the buffer is backscrolled
		for (x=0 ; x<linewidth ; x+=4)
			re.DrawChar ( (x+1)<<3, y, '^');
	
		y -= 8;
		rows--;
	}
	
	row = display;
	for (i=0 ; i<rows ; i++, y-=8, row--)
	{
		if (row < 0)
			break;
		if (current - row >= totallines)
			break;		// past scrollback wrap point
			
		text = text + (row % totallines)*linewidth;

		for (x=0 ; x<linewidth ; x++)
			re.DrawChar ( (x+1)<<3, y, text[x]);
	}

//ZOID
	// draw the download bar
	// figure out width
	if (cls.download) {
		if ((text = strrchr(cls.downloadname, '/')) != NULL)
			text++;
		else
			text = cls.downloadname;

		x = linewidth - ((linewidth * 7) / 40);
		y = x - strlen(text) - 8;
		i = linewidth/3;
		if (strlen(text) > i) {
			y = x - i - 11;
			strncpy(dlbar, text, i);
			dlbar[i] = 0;
			strcat(dlbar, "...");
		} else
			strcpy(dlbar, text);
		strcat(dlbar, ": ");
		i = strlen(dlbar);
		dlbar[i++] = '\x80';
		// where's the dot go?
		if (cls.downloadpercent == 0)
			n = 0;
		else
			n = y * cls.downloadpercent / 100;
			
		for (j = 0; j < y; j++)
			if (j == n)
				dlbar[i++] = '\x83';
			else
				dlbar[i++] = '\x81';
		dlbar[i++] = '\x82';
		dlbar[i] = 0;

		sprintf(dlbar + strlen(dlbar), " %02d%%", cls.downloadpercent);

		// draw it
		y = vislines-12;
		for (i = 0; i < strlen(dlbar); i++)
			re.DrawChar ( (i+1)<<3, y, dlbar[i]);
	}
//ZOID

// draw the input prompt, user text, and cursor if desired
	Con_DrawInput ();
}

#endif