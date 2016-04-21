

// -----------------------------------------------------------
//
// 72hr GDC. Bluescrn's entry. with no name yet.
//
// Game.cpp - Game init/deinit/main loop            
//
// -----------------------------------------------------------


#include "game.h"
#include <math.h>


float g_bounceTimer;

CFont font;

CTileMap map;

CPlayer player;

char *LevelNames[]=
{"",
 "first principles",
 "chocotastic!", 
 "red hot chilli peppers",
 "it gives you wings!",
 "not too taxing...",
 "let it all out!",
 "wet and wild",
 "chocolate bad... mmmkay?...",
 "the last level",
 "that's all, folks!"

};



char *introText[]=
{
	"Press Space To Start!",
	"(Esc to quit)",

	"Chocolate Bar",
	"Eat 'em all!",

	"Fizzy Drink",
	"Makes you burp",

	"Hot Chilli",
	"Burns!",

	"Energy Drink",
	"Gives you wings?",

	"Water",
	"is rather wet...",
	
	"Spikes",
	"Baaaad...",

	"Cracked Blocks",
	"Big players can destroy them",

	"Code+Gfx: Dave Reed (Bluescrn)",
	"Music+Sfx: Paul Barnard"
};

// -----------------------------------------------------------
// LoadTextures - Called whenever screenmode is changed
// -----------------------------------------------------------

void CGame::LoadTextures(void)
{
	GL_loadTexture(1,"font.png");

	GL_loadTexture(8,"blobbody.png");
	GL_loadTexture(9,"eyes.png");
	GL_loadTexture(10,"glow.png");
	GL_loadTexture(11,"glow2.png");

	GL_loadTexture(16,"tiles.png");
	GL_loadTexture(17,"parallax.png");

	GL_loadTexture(20,"choc.png");
	GL_loadTexture(21,"chilli.png");
	GL_loadTexture(22,"nrg.png");
	GL_loadTexture(23,"lager.png");

	GL_loadTexture(32,"spikes.png");

	GL_loadTexture(40,"fragment1.png");
	GL_loadTexture(41,"spark.png");
	GL_loadTexture(42,"blobsplat.png");


	GL_loadTexture(64,"water.png");
	GL_loadTexture(65,"airlock.png");


	SFX_load(2,"sfx\\chomp.wav",false);
	SFX_load(3,"sfx\\jump.wav",false);
	SFX_load(4,"sfx\\deflate.wav",false);
	SFX_load(5,"sfx\\get_ready.wav",false);
	SFX_load(6,"sfx\\stage_cleared.wav",false);
	SFX_load(7,"sfx\\power_up.wav",false);
	SFX_load(8,"sfx\\burp.wav",false);
	SFX_load(9,"sfx\\oww.wav",false);
	
	

	font.Init("font.png",32,32);
}


// -----------------------------------------------------------
// One-time init (loads audio)
// -----------------------------------------------------------
	
void CGame::Init(void)
{
	frame=0;	
	ENTITY_init();
}

		

void CGame::InitLevel(int level)
{
	ENTITY_reset();

	
	SFX_play(5);

	this->level=level;
	levelIntroTimer=-1;

	player.phys.vPos.x=8.2f;
	player.phys.vPos.y=9;
	
	char tmp[40];
	sprintf(tmp,"maps//%d.map",level);
	map.LoadMap(tmp,&player);
	
	player.Reset();

	GameState=GS_RUNNING;
	SubState=GSS_NORMAL;

	zoom=10;

	endTimer=0;
	
}




char message[80];
char message2[80];

void CGame::ShowMessage(char *text)
{
	messageTimer=-1;
	strcpy(message,text);
}

// -----------------------------------------------------------
// Main update function
// -----------------------------------------------------------

void CGame::Process(void)
{
	

	g_bounceTimer+=0.1f;

	



	frame++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	if (frame<4)
	{		
		if (frame==3)
		{
			SFX_load(1,"maintune.mp3",true);
			SFX_play(1);		
			GameState=GS_INITTITLE;
			
		}
		else
			font.Print("please wait... loading...",0,0,-0.4f,0,0);		
			return;
	}
	

	switch (GameState)
	{
		case GS_INITTITLE:
				introtimer=0.4f;
				InitLevel(0);
				GameState=GS_TITLE;

				titleTimer=0;
				titleFocus=-1;
				titleScroll=4.5f;
				
				
			break;

		case GS_TITLE:

				titleTimer-=0.01f;
				if (titleTimer<0)
				{
					titleTimer=2.5f;
					titleFocus++;

					if (titleFocus>8) titleFocus=0;
				}

				introtimer-=0.005f;
				if (introtimer<0) introtimer=0;

				{
					float targetScroll=4.5f+(6.0f*titleFocus);

					if (titleFocus==8)
					{
						if (titleScroll>4.5f)
							titleScroll-=0.2f;
					}
					else
					titleScroll=0.95f*titleScroll+
								0.05f*targetScroll;
				}

				map.Render(titleScroll,4.0f,10+(introtimer*120.0f),0);
				player.phys.radius=0.5f;
				ENTITY_process(&player);	// Now also renders the player

				font.Print("Nameless Team: 72hr Compo Entry",0,0.26f+introtimer,-0.55f,0,0);
				font.Print(GAMETITLE,0,0.066f,-0.28f+introtimer,0,FONT_COLOREFFECT);



				if (titleFocus<8)
					 font.Print(introText[titleFocus*2],0,-0.17f-introtimer,-0.35f,0,FONT_COLOREFFECT);
				else font.Print(introText[titleFocus*2],0,-0.25f-introtimer,-0.55f,0,0);

				font.Print(introText[titleFocus*2+1],0,-0.33f-introtimer,-0.55f,0,0);

				
			
				if (KEY_pressed(KEY_RESET)) 
				{
					timer=45;
					player.score=0;
					player.scoreAdd=0;
					InitLevel(1);
				}



				if (KEY_pressed(KEY_QUIT))  QUIT();

			break;

				

		case GS_RUNNING:


				// Check for win/lose conditions

				if (SubState==GSS_NORMAL)
				{
					timer-=(1.0f/60.0f);

					if (timer<0)
					{
						timer=0;
						ShowMessage("Time Up!");
						strcpy(message2,"g a m e  o v e r");
						SubState=GSS_TIMEUP;
					}

					if (player.bDead)
					{						
						ShowMessage("Ouch!");
						strcpy(message2," ");
						SubState=GSS_DEAD;
					}
					
					if (player.chocsLeft==0)
					{
						ShowMessage("Stage Cleared!");
						strcpy(message2,"Time Extend: 45 secs");
						timer+=45;

						SubState=GSS_WIN;
						SFX_play(6);
					}
				}

			
				float tzoom=6.0f+player.phys.radius*4.0f;
				zoom=(zoom*0.99f)+(tzoom*0.01f); //+=0.01f;

				map.Render(player.phys.vPos.x,player.phys.vPos.y,zoom, (player.xshift*10.0f));
				player.Update(1.0f/60.0f,&map);
				
				ENTITY_process(&player);	// Now also renders the player
				

				{
					char tmp[20];
					sprintf(tmp,"sc: %06d",player.score);
					font.Print(tmp,-0.2f,0.2f,-0.4f,0,0);
					sprintf(tmp,"time: %03d",((int)(timer)));
					font.Print(tmp,    0.2f,0.2f,-0.4f,0,(timer<10?FONT_COLOREFFECT:0));
				}
			//	player.phys.radius+=0.001f;

				levelIntroTimer+=0.02f;
				if (levelIntroTimer<3)
				{
					char tmp[30];
					sprintf(tmp,"level %d",level);

					float lit=levelIntroTimer;
					if (lit>0)
					{
						lit-=1;
						if (lit<0) lit=0;
					}
					
					font.Print(tmp,lit*0.3f,0.03f,-0.28f,0,FONT_COLOREFFECT);
					font.Print(LevelNames[level],-lit*0.5f,-0.05f,-0.38f,0,FONT_COLOREFFECT);

					font.Print("Space Bar resets level",0,-0.28f-fabs(lit)*0.3f,-0.48f,0,0);
				}

				// Reset level
				if (SubState==GSS_NORMAL)
				{
					if (KEY_pressed(KEY_RESET)) InitLevel(level);
				}
				else
				{
					endTimer+=0.01f;

					
					if (endTimer>2)
					{
						if (SubState==GSS_TIMEUP)
						{
							GameState=GS_INITTITLE;	
						}
						else
						{
							
							if (SubState==GSS_WIN) level++;

							InitLevel(level);
						}
					}
				}

				


				if (KEY_pressed(KEY_QUIT))  GameState=GS_INITTITLE;


				messageTimer+=0.02f;
				if (messageTimer<3)
				{
					float lit=messageTimer;
					if (lit>0)
					{
						lit-=1;
						if (lit<0) lit=0;
					}

					if (SubState==GSS_NORMAL)
						font.Print(message,0,-0.28f+(fabs(lit)*0.16f),-0.48f+(fabs(lit)*0.45f),0,FONT_COLOREFFECT);					
					else
					{
						font.Print(message,lit*0.5f, 0.0f,-0.3f,0,FONT_COLOREFFECT);					
						font.Print(message2,-lit*0.5f,-0.27f,-0.48f,0,FONT_COLOREFFECT);					
					}
				}


			break;
	}
	


	

	



}


// -----------------------------------------------------------
// Deinit function... probably won't get written....
// -----------------------------------------------------------

void CGame::Deinit(void)
{
	SFX_free(0);
}



