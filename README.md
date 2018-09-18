Pentagram Documentation
last changed: 09 November 2011 (GMT)

The latest version of this document can be found at http://pentagram.sourceforge.net/docs.php

# Contents
1. [Configuration](#1-configuration)
    1. [Quick guide](#1-quick-guide)
    2. [Advanced Configuration](#12-advanced-configuration)
    3. [Optional MIDI settings for Linux](#13-optional-midi-settings-for-linux)
2. [Movement and Keys](#2-movement-and-keys)
    1. [Movement](#21-movement)
    2. [Keyboard commands](#22-keyboard-commands)
3. [Other stuff](#3-other-stuff)
    1. [Console commands](#31-console-commands)
    2. [Supported games and versions](#32-supported-games-and-versions)
4. [Building SVN](#4-building-svn)
    1. [Build dependencies](#41-build-dependencies)
    2. [Linux/Unix](#42-linuxunix)
    3. [Windows](#43-windows)
    4. [Mac OS X](#44-mac-os-x)
5. [Changes to the Documentation](#5-changes-to-the-documentation)
    1. [09 November 2011](#51-09-november-2011)
    2. [30 October 2011](#52-30-october-2011)
    3. [5 January 2010](#53-5-january-2010)
    4. [4 September 2008](#54-september-2008)
    5. [3 August 2007](#55-3-august-2007)
    6. [19 February 2006](#56-19-february-2006)
    7. [22 May 2005](#57-22-may-2005)
    8. [21 May 2005](#58-21-may-2005)
    9. [14 October 2004](#59-1-october-2004)

---

# 1. Configuration
## 1. Quick guide
Pentagram needs a pentagram.ini file to point it to the game data files. The location of the file varies depending on the Operating System and in the case of Windows OS it also depends on the version of Windows.
* On Linux, pentagram.ini has to be in the ~/.pentagram directory.
* On Mac OS X, it has to be in the ~/Library/Application Support/Pentagram folder. Unfortunately ~/Library is hidden on OS X Lion, to open this folder click on the "Go" menu item of Finder while pressing 'Option/Alt'
* Windows 95, 98, 98 SE and ME save the ini to the folder where you installed Pentagram to (by default C:\Program Files\Pentagram)
* Windows 2000 and XP save pentagram.ini to APPDATA\Pentagram, which translates to C:\Documents and Settings\YourUsername\Application Data\Pentagram for an English version of Windows.
* Windows Vista and Windows 7 also save pentagram.ini to APPDATA\Pentagram, but on these this translates to C:\Users\YourUsername\AppData\Roaming\Pentagram
* On Windows 2000, XP, Vista and 7 you can quickly access this folder by executing shell:AppData\Pentagram in the "Run" entry of the Startmenu (or the "Run or Search" entry of the Windows Vista and 7 Startmenu)

A sample `pentagram.ini` is included below (this is all that is needed at the moment to run Pentagram). Comments are marked with # and are above the ini-entries. You can copy/paste these settings with the comments to your ini file as Pentagram will ignore lines beginning with #.

```
[u8]
# replace 'path to U8 directory' with the directory containing 'u8.exe'
path=path to U8 directory
```

## 1.2. Advanced Configuration
Right now Pentagram assumes some default settings that don't need to be set to make Pentagram work. The settings below are the default settings.

```
[pentagram]
defaultgame=u8
fullscreen=no
# Width of the game resolution.
scalex=320
# height of the game resolution.
scaley=240
# Select the scaler you want to use. Currently you can choose between
# point, bilinear, scale2x, 2xSaI, super2xSaI, supereagle, hq.
# hq scaler will automatically decide whether to use HQ2x, HQ3x or HQ4x
# depending on width and height.
scaler=2xSaI
# The game resolution is scaled to this width.
width=640
# The game resolution is scaled to this height.
height=480
bpp=32
# Enable this to use the BitStream Vera font for conversations and object descriptions.
ttf=no
# If you don't specify midi_driver, Pentagram will automatically select one,
# based on your platform.
# (Note: Windows will only work on Windows. CoreAudio and CoreMIDI only on Mac OS X.
# Alsa is Linux-only and UnixSeq will only work on Unix variants.)
# See further down for more options on Linux.
midi_driver=disabled/windows/fmopl/timidity/alsa/unixseq/coreaudio/coremidi/fluidsynth
#  Enter the path to the SoundFont for CoreAudio (optional) and FluidSynth (required).
soundfont=path to soundfont
# Conversation speed. Higher is slower.
textdelay=8
[u8]
# replace 'path to U8 directory' with the directory containing 'u8.exe'
path=path to U8 directory
# replace with the directory you want Pentagram to save games to.
# (no, loading of the original's savegames is not supported)
save=path to U8 savegames directory
# Enable this to skip the intro of U8 and initial conversation with Devon.
skipstart=no
# Gives the option to see the endgame animation.
endgame=no
# Gives the option to see the quotes (without having to watch the credits).
quotes=no
# Footsteps are audible.
footsteps=yes
# Enables post patch targeted jumps.
targetedjump=yes
# Enables cheating by clicking on the Avatar - currently cannot be disabled.
cheat=yes
```

You may also override most settings under `[pentagram]` by writing them under `[u8]`. Except for `defaultgame`, all can be overriden. Right now this is not necessary as we only support `U8` but if we start to support other games it might come in handy.

## 1.3. Optional MIDI settings for Linux
MIDI used on Linux might need some fine tuning in pentagram.ini to work correctly. Below are the settings you can configure.

```
[pentagram]
# If you don't specify midi_driver, Pentagram will automatically select one,
# based on your platform.
#(Note: Windows will only work on Windows. CoreAudio only on MacOSX.)
midi_driver=disabled/windows/fmopl/timidity/alsa/unixseq/coreaudio
# You may need to set timiditycfg if you choose midi_driver=timidity.
timiditycfg=path to timidity.cfg
# You may need to set alsa_port if you choose midi_driver=alsa.
alsa_port=alsa:port
# You may need to set unixseqdevice if you choose midi_driver=unixseq.
unixseqdevice=/dev/sequencer
```

---

# 2. Movement and Keys
## 2.1. Movement
* Double-click with the right mouse button will make the Avatar find a path to the selected spot.
* Left-click on NPCs or objects to identify them.
* Double-left-click on NPCs or objects to initiate conversations or use the item.
* On pressing the right mouse button down the Avatar walks or runs (depending on distance between cursor and Avatar).
* Pressing right and left mouse button, makes the Avatar jump or while running a running jump.

## 2.2. Keyboard commands
All key commands of Pentagram can be changed by editing `u8bindings.ini` or via the options menu in the game. We are distinguishing between default keyboard commands and debug keyboard commands that are used to help debugging the engine. At the moment you can't turn off the debug commands.

### Default keyboard commands
* `I` : Open backpack
* `Z` : Open paperdoll/status
* `R` : Recall
* `C` : Switches combat mode on/off
* `M` : Show/hide minimap
* `ESC` : Main menu
* `Ctrl+Q` : Quit without confirmation 
* `Backspace` : Close all open containers
* `F4` : Toggle fullscreen
* `F7` : Quicksave
* `F8` : Quickload

### Debug keyboard commands
* `~` (or) `F5` : Hides/restores console
* `Arrow keys` : Moves Avatar around the map (no walking)
* `Home` (and) `End` : Moves Avatar vertically 
* `T` : Outputs kernel/usecode/world/location stats to the console
* `E` : Toggles editor items
* `H` : Highlights any item you are "hitting", colliding with
* `F10` : Toggles "avatarInStasis"
* `F11` : Shapeviewer
* `F12` : Toggles frame by frame
* `Shift` : Quarter speed mode - needs to be held down
* `[` : Clears screen; multiple presses shows paint order
* `]` : above backwards

--------------------------------------------------------------------------------

# 3. Other stuff
## 3.1. Console commands
Pentagram has a debugging console with various useful commands. A selection is listed below.

### Console commands
* `GUIApp::quit` : Quit
* `ConsoleGump::toggle` : Open/close console
* `Cheat::heal` : Heals avatar
* `Cheat::equip` : Equip the avatar with magic armour
* `Cheat::maxstats` : Set STR/DEX/INT to 25
* `Cheat::toggleInvincibility` : Toggles avatar invincibility
* `MainActor::name <name>` : Set avatar name
* `MainActor::teleport <map> <x> <y> <z>` : teleport to coordinates
* `MiniMapGump::toggle` : Show/hide minimap

## 3.2. Supported games and versions
At the moment we only support the English versions of Ultima 8, both pre-patch and post-patch. The German, French, Spanish versions should also work, but are less well-tested. The Japanese version has a chance of working, but has hardly been tested at all.

---

# 4. Building SVN
## 4.1. Build dependencies
Before you build Pentagram, make sure you have the following tools/libraries.

* `SDL`;
* `SDL_ttf`;
* `zlib`;
* `libpng`;
* For Linux/Unix, you need `autoconf` and `automake`.

## 4.2. Linux/Unix
1. Run the `bootstrap` script to create the `configure` script: `./bootstrap`
2. Now run `configure`: `./configure`
3. Finally, start the build as usual: `make`

## 4.3. Windows
nothing here yet

## 4.4. Mac OS X
Under OS X, the dependencies are built into the application bundle as embedded libraries. Pentagram provides makefiles to build the dependencies appropriately. Internally, the OS X specific makefiles use normal unix-style configure & make to build Pentagram and all dependencies.
For the build, you should install XCode and the SDK for OS X 10.4 as it is currently the oldest supported version of OS X for the official builds. The build may still work on older versions or if the SDK is missing, but it will attempt target your version of OS X by default. Additional editing of system/macosx/archs.mk may be required for some configurations.
You also should ensure `/Developer/usr/bin` is in your `PATH` environment variable. You can do this by adding the following to `~/.profile` `export PATH=$PATH:/Developer/usr/bin`
Lastly, you may need to add configuration for autoconf and associated tools to work correctly. The following is a sample version of this configuration is in `system/macosx/autom4te.cfg`. Copy the contents of that file to `~/.autom4te.cfg` if the `bootstrap` script encounters problems.
After completing the system configuration listed above, perform the following steps to build Pentagram.

1. Run the `bootstrap` script to create the `configure` script: `./bootstrap`
2. Build all dependencies: `make -f Makefile.macosx deps`
3. Build Pentagram: `make -f Makefile.macosx build`
4. Build Pentagram.app: `make -f Makefile.macosx bundle`
5. Build disk image: `make -f Makefile.macosx image`
6. The disk image is located in the build directory and contains the universal binary

---

# 5. Changes to the Documentation
## 5.1. 09 November 2011
[1.2. Advanced Configuration](#12-advanced-configuration): More scalers and soundfont setting for FluidSynth and CoreAudio.

## 5.2. 30 October 2011
[1.2. Advanced Configuration](#12-advanced-configuration): added a lot of settings.
[2.1. Movement]](#21-movement): no more keypad movement and the not for Mac OS is no longer needed.
[2.2. Keyboard commands)](#22-keyboard-commands): updated and split in default and debug keys. Moved the Building SVN section to the bottom.

## 5.3. 5 January 2010
[4.4. Mac OS X](#44-mac-os-x): added additional information for builds.

## 5.4. 4 September 2008
[4.4. Mac OS X](#44-mac-os-x): replaced old build instructions.

## 5.5. 3 August 2007
[[3.1. Console commands](#31-console-commands): added (short) list of console commands.

## 5.6. 19 February 2006
[3.2. Supported games and versions](#32-supported-games-and-versions): updated status of non-English U8.

## 5.7. 22 May 2005
[1.2. Advanced Configuration](#12-advanced-configuration): some more configurations added.
[1.3. Optional MIDI settings for Linux](#13-optional-midi-settings-for-linux) added.

## 5.8. 21 May 2005
cleaned up outdated sections

## 5.9. 14 October 2004
[1.2. Advanced Configuration](#12-advanced-configuration) enable font replacement.
