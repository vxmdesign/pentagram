/*
 *	VarNodes.cpp -
 *
 *  Copyright (C) 2002 The Pentagram Team
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

#include "VarNodes.h"

/****************************************************************************
	PopVarNode
 ****************************************************************************/

/*void PopVarNode::fold(const uint32 end)
{
	PTRACE(("(PopVar)\tPOS: %4d\tOP: %04X offset: %04X\n", end, foldops[end].op(),
		foldops[end].offset));
	assert(foldops[end].deleted==false);

	_opcode = foldops[end].op(); // store the op locally
	_offset = foldops[end].offset; // store the offset locally

	switch(_opcode)
	{
		case 0x00: // popping a byte (1 byte)
			lnode  = new PushVarNode(PushVarNode::VT_BYTE, PushVarNode::DT_BP,
				foldops[end].i0);
			rtype=Type::T_VOID;
			break;
		case 0x01: // popping a word (2 bytes)
			lnode  = new PushVarNode(PushVarNode::VT_WORD, PushVarNode::DT_BP,
				foldops[end].i0);
			rtype=Type::T_VOID;
			break;
		case 0x02: // popping a dword (4 bytes)
			lnode  = new PushVarNode(PushVarNode::VT_DWORD, PushVarNode::DT_BP,
				foldops[end].i0);
			rtype=Type::T_VOID;
			break;
		case 0x08: // popping a result (4 bytes)
			lnode  = new PushVarNode(PushVarNode::VT_DWORD, PushVarNode::DT_RESULT, 0);
			rtype=Type::T_VOID;
			break;
		default: // die if we don't know the op
			printf("\nopcode %02X not supported in call to PopVarNode from offset %04X\n",
				_opcode, _offset);
			//printfolding();
			assert(false);
			break;
	}

	sint32 tempsize = lnode->rtype.size();
	assert(tempsize>0);
	grab_r(tempsize, end);
	assert(tempsize==0);
}*/


void PopVarNode::print_unk(Console &o, const uint32 isize) const
{
	assert(node!=0);
	Node::print_linenum_unk(o, isize);
	#if 0
	dtype.print_type_unk(o);
	o.Putchar(' ');
	#endif
	dtype.print_value_unk(o);
	o.Print(" = ");
	node->print_unk(o, isize);
}

void PopVarNode::print_asm(Console &o) const
{
	assert(node!=0);
	Node::print_linenum_asm(o);
	node->print_asm(o);
	o.Putchar('\n');
	Node::print_asm(o);
	switch(dtype.dtype())
	{
		case DataType::DT_BP:
			switch(dtype.type().type())
			{
				case Type::T_WORD:  o.Printf("pop\t\t");     dtype.print_value_asm(o); break;
				//case Type::T_DWORD: o.Printf("pop dword\t"); dtype.print_value_asm(o); break;
				default: assert(false);
			}
			break;
		default: assert(false);
	}
}

void PopVarNode::print_bin(OBufferDataSource &o) const
{
	assert(node!=0);
	Node::print_linenum_bin(o);
	node->print_bin(o);
	switch(dtype.dtype())
	{
		case DataType::DT_BP:
			switch(dtype.type().type())
			{
				case Type::T_WORD:  o.write1(0x01); dtype.print_value_bin(o); break;
				default: assert(false);
			}
			break;
		default: assert(false);
	}
}

bool PopVarNode::fold(Unit */*unit*/, std::deque<Node *> &nodes)
{
	assert(nodes.back()->rtype()==rtype());
	grab_n(nodes);
	fold_linenum(nodes);
	return true;
}


/****************************************************************************
	PushVarNode
 ****************************************************************************/

/*void PushVarNode::fold(const uint32 end)
{
	PTRACE(("(PushVar)\tPOS: %4d\tOP: %04X offset: %04X\n", end, foldops[end].op(),
		foldops[end].offset));
	assert(foldops[end].deleted==false);*/

/*	_opcode = foldops[end].op(); // store the op locally
	_offset = foldops[end].offset; // store the offset locally
*/
/*	switch(_opcode)
	{
		case 0x0D: // pushing a string (2 bytes)
			_vtype = VT_STRING;
			dtype  = DT_CHARS;
			strval = foldops[end].str;
			rtype=Type::T_STRING;
			break;
		case 0x3E: // pushing a byte var (2 bytes)
			_vtype  = VT_BYTE;
			dtype  = DT_BP;
			value  = foldops[end].i0;
			rtype=Type::T_BYTE;
			break;
		case 0x3F: // pushing a word var (2 bytes)
			_vtype  = VT_WORD;
			dtype  = DT_BP;
			value  = foldops[end].i0;
			rtype=Type::T_WORD;
			break;
		case 0x41: // pushing a string var (2 bytes)
			_vtype = VT_STRING;
			dtype  = DT_BP;
			value  = foldops[end].i0;
			rtype=Type::T_STRING;
			break;
		case 0x42: // pushing a list (2 bytes?)
			_vtype = VT_LIST;
			dtype  = DT_BPLIST;
			value  = foldops[end].i0;
			value2 = foldops[end].i1;
			assert(value2==2); // FIXME: incorrect, but a quick hack for my purposes
			rtype=Type::T_LIST;
			break;
		case 0x43: // pushing a slist (2 bytes?)
			_vtype  = VT_SLIST;
			dtype  = DT_BP;
			value  = foldops[end].i0;
			rtype=Type::T_SLIST;
			break;
		case 0x4E: // pushing a global (x bytes - round up to an even pair of bytes
			dtype  = DT_GLOBAL;
			global_offset = foldops[end].i0;
			global_size   = foldops[end].i1;
			switch(global_size)
			{
				case 0x01: _vtype = VT_BYTE;  rtype=Type::T_BYTE; break;
				case 0x02: _vtype = VT_WORD;  rtype=Type::T_WORD; break;
				case 0x03: _vtype = VT_DWORD; rtype=Type::T_DWORD; break;
//				case 0x04: _vtype = DWORD; rtype=Type::T_DWORD; break;
				// debugging, remove this and replace it with a _vtype=VAR when finished
				default: assert(false);
			}
			break;
		case 0x59: // pushing a pid (2 bytes - maybe)
			_vtype  = VT_VPID;
			dtype  = DT_DPID;
			value  = 0; // unused
			rtype=Type::T_PID;
			break;
		case 0x69: // pushing a string ptr (4 bytes)
			_vtype = VT_DWORD;
			dtype  = DT_BPSTRPTR;
			value  = 0x100-foldops[end].i0;
			rtype=Type::T_DWORD;
			break;
		case 0x6D: // pushing an address of a SP (4 bytes)
			_vtype = VT_DWORD;
			dtype  = DT_PRESULT;
			value  = 0; // unused
			rtype=Type::T_DWORD;
			break;
		case 0x6F: // pushing an address of a SP (4 bytes)
			_vtype = VT_DWORD;
			dtype  = DT_SPADDR;
			value  = 0x100-foldops[end].i0;
			rtype=Type::T_DWORD;
			break;
		case 0x79: // pushing a global address (4 bytes)
			if(crusader) // we're only a global for crusader
			{
				_vtype = VT_DWORD;
				dtype  = DT_SPADDR;
				value  = 0x100-foldops[end].i0;
				rtype=Type::T_DWORD;
			}
			else // not for u8
				assert(false);
			break;
		default: // die if we don't know the op
			printf("\nopcode %02X not supported in call to PushVarNode from offset %04X\n",
				_opcode, _offset);
			//printfolding();
			assert(false);
			break;
	}
}*/

void PushVarNode::print_unk(Console &o, const uint32 /*isize*/) const
{
	#if 0
	dtype.print_type_unk(o);
	o.Putchar(' ');
	#endif
	dtype.print_value_unk(o);
}

void PushVarNode::print_asm(Console &o) const
{
	Node::print_asm(o);
	switch(dtype.dtype())
	{
		case DataType::DT_BYTES:
			switch(dtype.type().type())
			{
				case Type::T_BYTE:  o.Printf("push byte\t");  dtype.print_value_asm(o); break;
				case Type::T_WORD:  o.Printf("push\t\t");     dtype.print_value_asm(o); break;
				case Type::T_DWORD: o.Printf("push dword\t"); dtype.print_value_asm(o); break;
				default: assert(false); // can't happen
			}
			break;
		case DataType::DT_BP:
			switch(dtype.type().type())
			{
				case Type::T_WORD:  o.Printf("push\t\t");     dtype.print_value_asm(o); break;
				case Type::T_DWORD: o.Printf("push dword\t"); dtype.print_value_asm(o); break;
				default: assert(false);
			}
			break;
		case DataType::DT_BPADDR:
			switch(dtype.type().type())
			{
				case Type::T_DWORD: o.Printf("push addr\t"); dtype.print_value_asm(o); break;
				default: assert(false); // can't happen
			}
			break;
		default: assert(false);
	}
}

void PushVarNode::print_bin(OBufferDataSource &o) const
{
	switch(dtype.dtype())
	{
		case DataType::DT_BYTES:
			switch(dtype.type().type())
			{
				case Type::T_BYTE:  o.write1(0x0A); dtype.print_value_bin(o); break;
				case Type::T_WORD:  o.write1(0x0B); dtype.print_value_bin(o); break;
				case Type::T_DWORD: o.write1(0x0C); dtype.print_value_bin(o); break;
				default: assert(false); // can't happen
			}
			break;
		case DataType::DT_BP:
			switch(dtype.type().type())
			{
				case Type::T_WORD:  o.write1(0x3F); dtype.print_value_bin(o); break;
				case Type::T_DWORD: o.write1(0x40); dtype.print_value_bin(o); break;
				default: assert(false);
			}
			break;
		case DataType::DT_BPADDR:
			switch(dtype.type().type())
			{
				case Type::T_DWORD: o.write1(0x4B); dtype.print_value_bin(o); break;
				default: assert(false); // can't happen
			}
			break;
		default: assert(false);
	}
}