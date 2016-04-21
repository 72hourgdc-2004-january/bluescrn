

// -----------------------------------------------------------
//
// 72hr GDC. Bluescrn's entry. with no name yet.
//
// Fonts.cpp - Font renderer. Probably bad use of time.
//
// -----------------------------------------------------------


#define FONT_COLOREFFECT 1

class CFont
{

public:
	void Init(char *file,int cw,int ch);			// Load a font
	
	void Print(char *str,float x,float y,float z,float rgb,int flags);

private:
	int texId;
	
	float cw,ch;

	float u0[256];
	float v0[256];
	float u1[256];
	float v1[256];
	float width[256];
};


