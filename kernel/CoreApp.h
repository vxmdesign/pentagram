/*
 *	CoreApp.h - Base application class that contains the minimal functionality
 *              to support pentagram tools
 *
 *  Copyright (C) 2002-2006  The Pentagram Team
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

#ifndef COREAPP_H
#define COREAPP_H

#include "intrinsics.h"
#include "Args.h"
#include "GameInfo.h"

class FileSystem;
class ConfigFileManager;
class SettingManager;
struct GameInfo;


#define GAME_IS_U8 (CoreApp::get_instance()->getGameInfo()->type == GameInfo::GAME_U8)
#define GAME_IS_REMORSE (CoreApp::get_instance()->getGameInfo()->type == GameInfo::GAME_REMORSE)
#define GAME_IS_REGRET (CoreApp::get_instance()->getGameInfo()->type == GameInfo::GAME_REGRET)
#define GAME_IS_CRUSADER (GAME_IS_REMORSE || GAME_IS_REGRET)


class CoreApp
{
public:
	ENABLE_RUNTIME_CLASSTYPE();
	CoreApp(int argc, const char* const* argv);
	virtual ~CoreApp();
	
	static CoreApp* get_instance() { return application; };

	virtual void run()=0; // FIXME: Need stub
	virtual void paint()=0; // probably shouldn't exist
	virtual bool isPainting() { return false; }
	
	virtual void ForceQuit() { isRunning=false; };

	//! Startup the application. This will prepare the application for run().
	//! Should call parent class' startup().
	void startup();

	//! Get current GameInfo struct
	GameInfo* getGameInfo() const { return gameinfo; }

	//! Get GameInfo for other configured game, or 0 for an invalid name.
	GameInfo* getGameInfo(Pentagram::istring game) const;
	
	virtual void helpMe();
	
	bool help()   const { return oHelp;   };
	bool quiet()  const { return oQuiet;  };
	bool vquiet() const { return oVQuiet; };

protected:

	//! Declare commandline arguments.
	//! Should call parent class' DeclareArgs().
	virtual void DeclareArgs();
	
	bool isRunning;
	
	std::map<Pentagram::istring, GameInfo*> games;
	GameInfo* gameinfo;
	
	// minimal system
	FileSystem* filesystem;
	ConfigFileManager* configfileman;
	SettingManager* settingman;

	Args parameters;
	
	static CoreApp* application;
	
private:
	int argc;
	const char* const* argv;

	//! start filesystem, kernel, config
	virtual void sysInit();

	//! parse commandline arguments
	void ParseArgs(int argc, const char* const*  argv);

	//! Fill a GameInfo struct for the give game name
	//! \param game The id of the game to check (from pentagram.cfg)
	//! \param gameinfo The GameInfo struct to fill
	//! \return true if detected all the fields, false if detection failed
	bool getGameInfo(Pentagram::istring& game, GameInfo* gameinfo);

	//! setup default virtual paths (@home, @data)
	void setupVirtualPaths();

	//! load configuration files
	void loadConfig();

protected:
	void setupGameList();

	//! return default game
	//! \return 0 if no default game (implies go to Pentagram Menu)
	GameInfo* getDefaultGame();

	//! Setup up a game
	//! \return false if failed (implies go to Pentagram Menu)
	bool setupGame(GameInfo* info);

	//! kill current gameinfo
	void killGame();

	//! Setup the virtual game paths for the current game (set in gameinfo)
	//! Specifically, @game and @work
	void setupGamePaths(GameInfo* gameinfo);

	std::string oGamename;
	bool oHelp;
	bool oQuiet;
	bool oVQuiet;
};

#endif

