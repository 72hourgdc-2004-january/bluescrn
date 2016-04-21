

// -----------------------------------------------------------
//
// 72hr GDC. Bluescrn's entry. with no name yet.
//
// Main.h   - Init SDL+GL screen, init audio, handle input
//            Also basic interface to DevIL and FMod
//
// -----------------------------------------------------------


enum
{
	KEY_FIRE=0,
	KEY_RESET,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_QUIT,
	NUM_KEYS
};


int  GL_loadTexture(int tex,char *filename);	// Load texture using DevIL
void GL_bindTexture(int tex);


void SFX_load(int id,char *filename,bool bLoop);
void SFX_play(int id);
void SFX_free(int id);

void SFX_play2(int id,int freq);

bool KEY_pressed(int key);
bool KEY_state(int key);

void QUIT(void);