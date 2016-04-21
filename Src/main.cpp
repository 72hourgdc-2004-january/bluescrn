
// -----------------------------------------------------------
//
// 72hr GDC. Bluescrn's entry. with no name yet.
//
// Main.cpp - Init SDL+GL screen, init audio, handle input
//            Also basic interface to DevIL and FMod
//
// -----------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>

#include <sdl.h>


#include "game.h"


#define SC_WIDTH  320
#define SC_HEIGHT 240

#define BPP 16

//#define FULLSCREEN
//#define WINDOWED

int texNames[256];			// OpenGL texture IDs


bool keystate[NUM_KEYS];
bool keypressed[NUM_KEYS];

// -----------------------------------------------------------
// Init OpenGL
// -----------------------------------------------------------

void InitGL(int w,int h)
{
	glViewport(0,0,w,h);
	glClearColor(0,0.0f,0.0f,0);

	glClearDepth(1);
    glDepthFunc(GL_LESS);				
    glEnable(GL_DEPTH_TEST);		

    glShadeModel(GL_SMOOTH);		

    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();								
    gluPerspective(70.0f,640.0f/480.0f,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);
}



// -----------------------------------------------------------
// Video mode setup (for toddling fullscreen/windowed)
// -----------------------------------------------------------

void SetVideoMode(int w,int h,int flags)
{
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, BPP);


	if ( SDL_SetVideoMode(w, h, BPP, SDL_OPENGL | flags) == NULL ) 
	{
		fprintf(stderr, "Failed to init OpenGL screen: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}
	else
	{
		SDL_ShowCursor(SDL_DISABLE);
		InitGL(w, h);
	}
}


// -----------------------------------------------------------
// Entry Point / Main Loop
// -----------------------------------------------------------

bool bQuit=false;

void QUIT(void) {bQuit=true;}

CGame *game;


float g_oldTime;
int GetAccurateTickCount()
{
	LARGE_INTEGER counterTicks;
	LARGE_INTEGER counterFreq;
	double        tickCount;

	QueryPerformanceCounter(&counterTicks);
	QueryPerformanceFrequency(&counterFreq);

	tickCount=((double)(counterTicks.QuadPart))*1000.0f;
	tickCount/=((double)(counterFreq.QuadPart));

	return (int)(tickCount);
}


int main(int argc, char **argv)
{

	

	int  vidflags=0;	
	bool bSetVidMode=true;
	float t=0;


	// Init DevIL
	// ------------------------------------

	ilInit();
	
		

	// Init FMOD
	// ------------------------------------
	
	FSOUND_Init(44100, 32, FSOUND_INIT_USEDEFAULTMIDISYNTH);


	// Init SDL (video only)
	// ------------------------------------

	if (SDL_Init(SDL_INIT_VIDEO)<0) 
	{
		fprintf(stderr,"Failed to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	// Create a 640x480 screen and OpenGL
	// rendering context
	// ------------------------------------
	
#ifdef FULLSCREEN
	vidflags=SDL_FULLSCREEN;
#else 
	#ifdef WINDOWED
		
	#else
		// Choice... Win32 only!		
		int i=MessageBox(NULL,"Run fullscreen? (F1 toggles full-screen/windowed in-game)",GAMETITLE,MB_YESNO);

		if (i==6) vidflags=SDL_FULLSCREEN;
	#endif
#endif


	

	for (int n=0;n<NUM_KEYS;n++) keystate[n]=false;


	SDL_WM_SetCaption("(F1 toggles full-screen mode)", NULL);

  

	game=new CGame;
	  
			
	game->Init();
			
	// Main Loop - with keyboard handling
	// ------------------------------------

	while(!bQuit)
	{

		while(GetAccurateTickCount()<(g_oldTime+(1000/80)));
		g_oldTime=GetAccurateTickCount();


		// Video 
		if (bSetVidMode)
		{
			SetVideoMode(SC_WIDTH,SC_HEIGHT,vidflags);

			ilShutDown();
			ilInit();
			iluInit();
			ilutRenderer(ILUT_OPENGL);

			// load bitmaps
			game->LoadTextures();


			bSetVidMode=false;
		}


		game->Process();		
		SDL_GL_SwapBuffers();


		
		SDL_Event event;

		for (int k=0;k<NUM_KEYS;k++) keypressed[k]=false;

		while (SDL_PollEvent(&event)) 
		{

			if (event.type==SDL_QUIT) bQuit=TRUE;
									
			if (event.type==SDL_KEYDOWN) 		
			{
			//	if (event.key.keysym.sym==SDLK_ESCAPE) bQuit=TRUE;                      

				if (event.key.keysym.sym==SDLK_F1)
				{
					if (vidflags==0) vidflags=SDL_FULLSCREEN;
							    else vidflags=0;

					bSetVidMode=true;
				}

			}

			if (event.type==SDL_KEYDOWN || event.type==SDL_KEYUP) 		
			{
				bool s=false;
				if (event.type==SDL_KEYDOWN) s=true;

				if (event.key.keysym.sym==SDLK_LEFT)  keystate[KEY_LEFT]=s;
				if (event.key.keysym.sym==SDLK_RIGHT) keystate[KEY_RIGHT]=s;
				if (event.key.keysym.sym==SDLK_UP)	 keystate[KEY_UP]=s;
				if (event.key.keysym.sym==SDLK_DOWN)  keystate[KEY_DOWN]=s;
				if (event.key.keysym.sym==SDLK_SPACE)  keystate[KEY_RESET]=s;

				if (s)
				{
				  if (event.key.keysym.sym==SDLK_SPACE)  keypressed[KEY_RESET]=true;
				  if (event.key.keysym.sym==SDLK_ESCAPE)  keypressed[KEY_QUIT]=true;
				}
			}


        }

	}



	game->Deinit();

 
	ilShutDown();
	FSOUND_Close();
  
	SDL_Quit();
	return 0;
}



// -----------------------------------------------------------
// Texture Loading
// -----------------------------------------------------------

void GL_bindTexture(int tex)
{
	glBindTexture(GL_TEXTURE_2D,texNames[tex]);
}


int GL_loadTexture(int tex,char *filename)
{	
	char fn2[512];
	int r;

	sprintf(fn2,"data\\%s",filename);
	

	//texNames[tex]=ilutGLLoadImage(fn2);
	unsigned int tmp;
	ilGenImages( 1, &tmp );
	ilBindImage( tmp );	
	ilLoadImage( fn2 );

	texNames[tex]=ilutGLBindMipmaps();

	GL_bindTexture(tex);	

	return r;
}




FSOUND_SAMPLE *sfx[32];

void SFX_load(int id,char *filename,bool bLoop)
{
	char filename2[512];
	sprintf(filename2,"data\\%s",filename);

	sfx[id] = FSOUND_Sample_Load(FSOUND_UNMANAGED, filename2, FSOUND_NORMAL , 0,0);

	if (bLoop)
	{
		FSOUND_Sample_SetLoopPoints(sfx[id], 0,FSOUND_Sample_GetLength(sfx[id]));	
		FSOUND_Sample_SetMode(sfx[id], FSOUND_LOOP_NORMAL);
	}
}


void SFX_play(int id)
{
	FSOUND_PlaySound(FSOUND_FREE, sfx[id]);
}

void SFX_play2(int id,int freq)
{
	FSOUND_SetFrequency(FSOUND_PlaySound(FSOUND_FREE, sfx[id]),freq);
}




void SFX_free(int id)
{
	FSOUND_Sample_Free(sfx[id]);
}




bool KEY_state(int key)
{
	return keystate[key];
}

bool KEY_pressed(int key)
{
	return keypressed[key];
}