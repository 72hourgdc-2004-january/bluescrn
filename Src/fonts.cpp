

// -----------------------------------------------------------
//
// 72hr GDC. Bluescrn's entry. with no name yet.
//
// Fonts.cpp - Font renderer. Probably bad use of time.
//
// -----------------------------------------------------------

#include "game.h"

#include "math.h"


int nextTexId=255;


void CFont::Init(char *file,int cw,int ch)
{
	unsigned char *pData;
	int chr,cx,cy,px,py,ww;

	int w=cw*16;
	int h=ch*16;


	float one_pixel=1.0f/((float)(w));

	this->cw=((float)(cw))*one_pixel;
	this->ch=((float)(ch))*one_pixel;


	texId=nextTexId--;

	GL_loadTexture(texId,file);

	// Now we need to load a 2nd copy... as I can't find a sensible way
	// of accessing textures
	{
		char tmp[512];
		unsigned int i;
		sprintf(tmp,"data\\%s",file);

		ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
		ilGenImages(1,&i);
		ilBindImage(i);
		ilLoadImage(tmp);

		pData=ilGetData();


		for(cy=0;cy<16;cy++)
		{
		
			for(cx=0;cx<16;cx++)
			{

				ww=0;

				for(px=0;px<cw;px++)
				{
					unsigned char *pCol=pData+(4* (px+((cx*cw)+(cy*w*ch))) );

					for(py=0;py<ch;py++)
					{
						if (pCol[3]>24) ww=px;


						pCol+=(4*w);
					}
				}

	
				chr=(cy*16)+cx;

				if (chr==32) ww=cw/3;
				
				ww+=1;

				width[chr]=((float)(ww))*one_pixel;

				u0[chr]=(one_pixel*0.5f)+((float)(cx*cw))*one_pixel;
				u1[chr]=(one_pixel*0.5f)+((float)(ww+cx*cw))*one_pixel;
				v0[chr]=(one_pixel*0.5f)+((float)(cy*ch))*one_pixel;
				v1[chr]=(one_pixel*0.5f)+((float)((ch-1)+cy*ch))*one_pixel;

			}
		}


		ilDeleteImages(1,&i);
	}


	
}



void CFont::Print(char *str,float x,float y,float z,float rgb,int flags)
{


	int cc=0,chr,c;
	int l=strlen(str);

	float pack=3.0f/512.0f;		// pack the chars closer together...

		
	
	GL_bindTexture(texId);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);		


	
	
	float pl=0,hch=ch/2.0f;

	// Nasty hack to centre everything...
	for(c=0;c<l;c++)
	{		
		chr=str[c];
		pl+=width[chr]-pack;
	}
	x-=pl/2.0f;

	glColor3f(1,1,1);

	glLoadIdentity();			
	glPushMatrix();

	// Draw it!
	for(c=0;c<l;c++)
	{		
		chr=str[c];

		float s=(0.05f*sin(g_bounceTimer+c));		
		float ww=width[chr];
		float hww=ww/2.0f;

		glPopMatrix();
		glPushMatrix();
		
		glTranslatef(x,y,z);	
		glScalef(1.0f+s,1.0f+s,1);

		glRotatef(s*60.0f,0,1,0);

		glTranslatef(hww,hch,0);	
	
		if (flags&FONT_COLOREFFECT)
		{
			float ss=s*4.0f;
			glColor3f(0.8f+ss,0.8f+ss,0.8f+ss);
		}
		

		glBegin(GL_POLYGON);						

		glTexCoord2f(u0[chr],-v0[chr]);
		glVertex3f( -hww,hch,0);		
					
		glTexCoord2f(u1[chr],-v0[chr]);
		glVertex3f( hww,hch,0);		
					
		glTexCoord2f(u1[chr],-v1[chr]);
		glVertex3f(hww,-hch,0);
					
		glTexCoord2f(u0[chr],-v1[chr]);
		glVertex3f(-hww,-hch,0);		
			
		glEnd();							

		x+=ww;
		x-=pack;
	}

	glPopMatrix();

    glEnable(GL_DEPTH_TEST);		


}