

// -----------------------------------------------------------
//
// 72hr GDC. Bluescrn's entry. with no name yet.
//
// Game.cpp - Game init/deinit/main loop            
//
// -----------------------------------------------------------

#ifndef _GAME_H_
#define _GAME_H_

#include <glheaders.h>
#include "il\il.h"		
#include "il\ilu.h"
#include "il\ilut.h"
#include "fmod.h"

#define	 GAMETITLE	"Size Matters?"   // Probably will want changing....

extern float g_bounceTimer;

#include "main.h"
#include "fonts.h"

#include "collision.h"
#include "scroller.h"
#include "player.h"
#include "entities.h"

enum
{
	GS_INITTITLE=0,			// Game states
	GS_TITLE,
	GS_RUNNING
};

enum
{
	GSS_NORMAL=0,
	GSS_WIN,
	GSS_DEAD,
	GSS_TIMEUP
};

class CGame
{
	public:		
		void LoadTextures(void);
		void Init(void);
		void Process(void);
		void Deinit(void);

		void InitLevel(int level);

		void ShowMessage(char *text);

		
	private:
		int GameState;
		int SubState;

		int   level;
		float levelIntroTimer;

		int   frame;
		float introtimer;
		float zoom;

		float timer;

		float titleTimer;
		int   titleFocus;
		float titleScroll;

		float messageTimer;


		float endTimer;
		
		
};


#endif