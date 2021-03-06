/*
Copyright (C) 2003-2006 The Pentagram team

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

#include "pent_include.h"

#include "GameInfo.h"

#include "IDataSource.h"
#include "ODataSource.h"
#include "util.h"


struct GameTypeDesc {
	const char* shortname;
	const char* longname;
};
struct GameLangDesc {
	char letter;
	char usecodeletter;
	const char* name;
};

// Keep order the same as the GameType enum!
static GameTypeDesc gametypes[] = {
	{ "", "" },
	{ "ultima8", "Ultima VIII: Pagan" },
	{ "remorse", "Crusader: No Remorse" },
	{ "regret", "Crusader: No Regret" },
	{ "pentmenu", "Pentagram Menu" },
	{ 0, 0 }
};

// Keep order the same as the GameLanguage enum!
static GameLangDesc gamelangs[] = {
	{ 0, 0, "unknown" },
	{ 'e', 'e', "English" },
	{ 'f', 'f', "French" },
	{ 'g', 'g', "German" },
	{ 'e', 'e', "Spanish" },
	{ 'e', 'j', "Japanese" },
	{ 0, 0 }
};


GameInfo::GameInfo()
	: type(GAME_UNKNOWN), version(0), language(GAMELANG_UNKNOWN)
{
	for (int i = 0; i < 16; ++i)
		md5[i] = 0;
}

char GameInfo::getLanguageFileLetter() const
{
	switch (type) {
	case GAME_U8:
	{
		unsigned int l = static_cast<unsigned int>(language);
		assert(l < (sizeof(gamelangs)/sizeof(gamelangs[0]))-1);

		return gamelangs[l].letter;
	}
	case GAME_REMORSE:
	case GAME_REGRET:
		return 'e';
	default:
		return 0;
	}
}

char GameInfo::getLanguageUsecodeLetter() const
{
	switch (type) {
	case GAME_U8:
	{
		unsigned int l = static_cast<unsigned int>(language);
		assert(l < (sizeof(gamelangs)/sizeof(gamelangs[0]))-1);

		return gamelangs[l].usecodeletter;
	}
	case GAME_REMORSE:
	case GAME_REGRET:
		return 'e';
	default:
		return 0;
	}
}

std::string GameInfo::getLanguage() const
{
	unsigned int l = static_cast<unsigned int>(language);
	assert(l < (sizeof(gamelangs)/sizeof(gamelangs[0]))-1);

	return gamelangs[l].name;
}

std::string GameInfo::getGameTitle() const
{
	unsigned int t = static_cast<unsigned int>(type);
	assert(t < (sizeof(gametypes)/sizeof(gametypes[0]))-1);

	return gametypes[t].longname;
}

std::string GameInfo::getPrintableVersion() const
{
	char buf[32];
	sprintf(buf, "%d.%02d", version/100, version%100);
	return buf;
}

std::string GameInfo::getPrintDetails() const
{
	std::string ret;

	std::string title = getGameTitle();
	if (title == "") title = "Unknown";

	ret = title + ", ";

	std::string lang = getLanguage();
	if (lang == "") lang = "Unknown";
	ret += lang;

	if (type != GAME_PENTAGRAM_MENU) {
		// version, md5 don't make sense for the pentagram menu

		ret += ", version ";
		ret += getPrintableVersion();

		ret += ", md5 ";
		ret += getPrintableMD5();
	}

	return ret;
}


std::string GameInfo::getPrintableMD5() const
{
	std::string ret;

	char buf[33];
	for (int i = 0; i < 16; ++i) {
		sprintf(buf+2*i, "%02x", md5[i]);
	}

	ret = buf;

	return ret;
}

bool GameInfo::match(GameInfo& other, bool ignoreMD5) const
{
	if (type != other.type) return false;
	if (language != other.language) return false;
	if (version != other.version) return false;

	if (ignoreMD5) return true;

	return (std::memcmp(md5, other.md5, 16) == 0);
}

void GameInfo::save(ODataSource* ods)
{
	unsigned int l = static_cast<unsigned int>(language);
	assert(l < (sizeof(gamelangs)/sizeof(gamelangs[0]))-1);
	unsigned int t = static_cast<unsigned int>(type);
	assert(t < (sizeof(gametypes)/sizeof(gametypes[0]))-1);

	std::string game = gametypes[t].shortname;
	std::string lang = gamelangs[l].name;

	char buf[16];
	sprintf(buf, "%d", version);
	std::string ver = buf;
	std::string md5 = getPrintableMD5();

	std::string d = game + "," + lang + "," + ver + "," + md5 + "\n";
	ods->write(d.c_str(), d.size());
}

bool GameInfo::load(IDataSource* ids, uint32 version)
{
	std::string s;
	std::vector<std::string> parts;

	ids->readline(s);
	Pentagram::SplitString(s, ',', parts);
	if (parts.size() != 4) return false;

	int i = 0;
	while (gametypes[i].shortname) {
		if (parts[0] == gametypes[i].shortname) {
			type = static_cast<GameType>(i);
			break;
		}
		i++;
	}
	if (!gametypes[i].shortname) return false;

	i = 0;
	while (gamelangs[i].name) {
		if (parts[1] == gamelangs[i].name) {
			language = static_cast<GameLanguage>(i);
			break;
		}
		i++;
	}
	if (!gamelangs[i].name) return false;

	this->version = std::strtol(parts[2].c_str(), 0, 0);

	for (int i = 0; i < 16; ++i) {
		char buf[3];
		buf[0] = parts[3][2*i];
		buf[1] = parts[3][2*i+1];
		buf[2] = 0;
		long x = std::strtol(buf, 0, 16);
		md5[i] = static_cast<uint8>(x);
	}

	return true;
}
