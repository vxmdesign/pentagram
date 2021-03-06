/*
 *  Copyright (C) 2004-2005 The Pentagram Team
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
#include "TreasureLoader.h"

#include "ConfigFileManager.h"
#include "util.h"

TreasureLoader::TreasureLoader()
{

}

TreasureLoader::~TreasureLoader()
{

}

void TreasureLoader::loadDefaults()
{
	ConfigFileManager* config = ConfigFileManager::get_instance();
	std::map<Pentagram::istring,std::string> lootkeyvals;

	// load default treasure types
	lootkeyvals = config->listKeyValues("game/treasure");
	std::map<Pentagram::istring,std::string>::iterator defaultiter;
	for (defaultiter = lootkeyvals.begin();
		 defaultiter != lootkeyvals.end(); ++defaultiter)
	{
		TreasureInfo ti;
		bool ok = internalParse(defaultiter->second, ti, true);
		if (ok) {
			defaultTreasure[defaultiter->first] = ti;
		} else {
			perr << "Failed to parse treasure type '" << defaultiter->first
				 << "': " << defaultiter->second << std::endl;
		}
	}

}

bool TreasureLoader::parse(std::string desc,
						   std::vector<TreasureInfo>& treasure)
{
	treasure.clear();

	std::vector<std::string> tr;
	Pentagram::SplitString(desc, ';', tr);

	TreasureInfo ti;
	for (unsigned int i = 0; i < tr.size(); ++i) {
//		pout << "parse: item=" << tr[i] << std::endl;
		if (internalParse(tr[i], ti, false)) {
			treasure.push_back(ti);
		} else {
			return false;
		}		
	}

	return true;
}

bool TreasureLoader::internalParse(std::string desc, TreasureInfo& ti,
								   bool loadingDefault)
{
	ti.special = "";
	ti.chance = 1;
	ti.map = 0;
	ti.shapes.clear();
	ti.frames.clear();
	ti.mincount = ti.maxcount = 1;

	bool loadedDefault = false;

	std::vector<std::pair<std::string, std::string> > kv;
	Pentagram::SplitStringKV(desc, ' ', kv);

	for (unsigned int i = 0; i < kv.size(); ++i) {
		std::string key = kv[i].first;
		std::string val = kv[i].second;
//		pout << "internalParse: key=" << key << " val=" << val << std::endl;

		if (key == "shape") {
			if (!parseUInt32Vector(val, ti.shapes))
				return false;
		} else if (key == "frame") {
			if (!parseUInt32Vector(val, ti.frames))
				return false;
		} else if (key == "count") {
			if (!parseUIntRange(val, ti.mincount, ti.maxcount)) {
				int x;
				if (!parseInt(val, x))
					return false;
				ti.mincount = ti.maxcount = x;
			}
		} else if (key == "chance") {
			if (!parseDouble(val, ti.chance))
				return false;
		} else if (key == "map") {
			if (val.size() > 1 && val[0] == '!')
				val[0] = '-'; // HACK: invert map for 'not this map'
			if (!parseInt(val, ti.map))
				return false;
		} else if (key == "special" && loadingDefault) {
			ti.special = val;
		} else if (key == "type" && !loadingDefault) {
			if (loadedDefault)
				return false;
			std::map<Pentagram::istring, TreasureInfo>::iterator iter;
			iter = defaultTreasure.find(val);
			if (iter != defaultTreasure.end())
				ti = iter->second;
			else
				return false;
			loadedDefault = true;
		} else if (key == "mult" && !loadingDefault) {
			if (!loadedDefault) return false;
			unsigned int minmult, maxmult;
			if (!parseUIntRange(val, minmult, maxmult)) {
				int x;
				if (!parseInt(val, x))
					return false;
				minmult = maxmult = x;
			}
			ti.mincount *= minmult;
			ti.maxcount *= maxmult;
		} else {
			return false;
		}
	}

	return true;
}

bool TreasureLoader::parseUInt32Vector(std::string val,
									   std::vector<uint32>& vec)
{
	vec.clear();

	std::string::size_type pos;
	while (!val.empty()) {
		pos = val.find(',');
		std::string item = val.substr(0, pos);

		std::string::size_type itempos = val.find('-');
		if (itempos != std::string::npos) {
			unsigned int min, max;
			if (!parseUIntRange(item, min, max))
				return false;
			for (unsigned int i = min; i <= max; ++i)
				vec.push_back(i);
		} else {
			int x;
			if (!parseInt(item, x))
				return false;
			vec.push_back(x);
		}
		
		if (pos != std::string::npos) pos++;
		val.erase(0, pos);
	}

	return true;
}

bool TreasureLoader::parseUIntRange(std::string val,
								   unsigned int& min, unsigned int& max)
{
	std::string::size_type pos = val.find('-');
	if (pos == 0 || pos == std::string::npos || pos+1 >= val.size())
		return false;
	int t1, t2;
	bool ok = true;
	ok &= parseInt(val.substr(0, pos), t1);
	ok &= parseInt(val.substr(pos+1), t2);
	if (ok) {
		min = t1;
		max = t2;
	}
	return ok;
}

bool TreasureLoader::parseDouble(std::string val, double& d)
{
	// TODO: error checking
	d = std::strtod(val.c_str(), 0);
	return true;
}

bool TreasureLoader::parseInt(std::string val, int& i)
{
	// TODO: error checking
	i = std::strtol(val.c_str(), 0, 0);
	return true;
}
