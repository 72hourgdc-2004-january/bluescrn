


// -----------------------------------------------------------
//
// 72hr GDC. Bluescrn's entry. still with no name.
//
// Player.cpp - Control system, special abilities, etc
//
// -----------------------------------------------------------

#include "game.h"
#include <math.h>


extern CGame *game;

void CPlayer::Reset(void)
{
	phys.vVel.x=phys.vVel.y=0;
	angle=0;
	phys.radius=0.25f; 
	phys.bounce=0;
	xshift=yshift=0;
	
	targetRadius=0.25f;

	bChilli=false;
	bWings=false;

	bDead=false;

}

CPlayer::CPlayer()
{
	Reset();
	
}


void CPlayer::GetPickup(int type)
{
	if (bDead) return;
	

	if (type==ENTITY_CHOC)
	{
//		SFX_play(2);
		SFX_play2(2,60000.0f-(phys.radius*10000.0f));				
		scoreAdd+=50;
		targetRadius+=0.125f;
		chocsLeft--;
	}
	
	if (type==ENTITY_CHILLI)
	{
		SFX_play(7);		 // Power Up!

		scoreAdd+=250;
		game->ShowMessage("Red Hot Chilli Power!");

		
		bChilli=true;		
	}

	if (type==ENTITY_NRG)
	{
		scoreAdd+=250;
		SFX_play(7);		 // Power Up!

		game->ShowMessage("Gives Me Wings!");

		bWings=true;
	}

	if (type==ENTITY_LAGER)
	{
		SFX_play(8);		 // Burp;

		scoreAdd+=100;
		targetRadius-=0.5f;
		if (targetRadius<0.25f) targetRadius=0.25f;

		game->ShowMessage("Buurrrrp!");
	}
	
	
	if (type==ENTITY_SPIKES)
	{
		SFX_play(9);		 // Oww!;

		scoreAdd+=100;
		targetRadius-=0.5f;
		if (targetRadius<0.25f) targetRadius=0.25f;

		bDead=true;

		ENTITY_addFragments(phys.vPos.x,phys.vPos.y,2);
	}
}


void CPlayer::Render()
{
	// Draw the player sprite...
	
	if (bDead) return;
	

	float r,sc,ang;
	int i;

	ang=g_bounceTimer*8;

	if (bChilli)
	for(i=0;i<3;i++)
	{
		ang+=120;

		glPushMatrix();
		glTranslatef(phys.vPos.x,phys.vPos.y,-0.5f);	
		glRotatef(ang,0,0,1);
		
		sc=1.0f+(0.3f*sin(((float)(i))+g_bounceTimer*1.0f));
		glScalef(sc,sc,sc);
		glColor3f(0.6f,0.2f,0.1f);

		r=phys.radius*1.6f;
		GL_bindTexture(11);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_ONE,GL_ONE);
		glEnable(GL_BLEND);			
		glBegin(GL_POLYGON);
			glTexCoord2f(0,0);
			glVertex3f(-r,-r,0);
			glTexCoord2f(1,0);
			glVertex3f( r,-r,0);
			glTexCoord2f(1,1);
			glVertex3f( r, r,0);
			glTexCoord2f(0,1);
			glVertex3f(-r, r,0);
		glEnd();
		glPopMatrix();
	}


	if (bWings)
	for(i=0;i<3;i++)
	{
		ang+=120;

		glPushMatrix();
		glTranslatef(phys.vPos.x,phys.vPos.y,-0.5f);	
		glRotatef(ang,0,0,1);
		
		sc=1.0f+(0.3f*sin(((float)(i))+g_bounceTimer*1.0f));
		glScalef(sc,sc,sc);
		glColor3f(0.3f,0.3f,1.0f);

		r=phys.radius*2.2f;
		GL_bindTexture(10);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_ONE,GL_ONE);
		glEnable(GL_BLEND);			
		glBegin(GL_POLYGON);
			glTexCoord2f(0,0);
			glVertex3f(-r,-r,0);
			glTexCoord2f(1,0);
			glVertex3f( r,-r,0);
			glTexCoord2f(1,1);
			glVertex3f( r, r,0);
			glTexCoord2f(0,1);
			glVertex3f(-r, r,0);
		glEnd();
		glPopMatrix();
	}
	

	glPushMatrix();
	glTranslatef(phys.vPos.x,phys.vPos.y,-0.5f);	
	glRotatef(angle,0,0,1);
	r=phys.radius;	

	GL_bindTexture(8);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	if (bChilli)
		glColor3f(1,0.7f,0);
	else
		glColor3f(1,1,1);
	
	glBegin(GL_POLYGON);
		glTexCoord2f(0,0);
		glVertex3f(-r,-r,0);
		glTexCoord2f(1,0);
		glVertex3f( r,-r,0);
		glTexCoord2f(1,1);
		glVertex3f( r, r,0);
		glTexCoord2f(0,1);
		glVertex3f(-r, r,0);
	glEnd();

	


	glPopMatrix();
	glPushMatrix();

	GL_bindTexture(9);
	float rr=r*1.0f;
	float txshift=phys.vVel.x*4.0f;
	if (txshift>0.7f)  txshift=0.7f;
	if (txshift<-0.7f) txshift=-0.7f;
	xshift=xshift*0.7f+txshift*0.3f;
	
	float tyshift=phys.vVel.y*4.0f;
	if (tyshift>0.7f)  tyshift=0.7f;
	if (tyshift<-0.7f) tyshift=-0.7f;
	yshift=yshift*0.7f+tyshift*0.3f;

	glTranslatef(phys.vPos.x+(r*xshift),phys.vPos.y+(r*yshift)-(r*0.8f),-0.4f);	
	
	
	glBegin(GL_POLYGON);
	glTexCoord2f(0,0);
	glVertex3f(-rr,-rr,0);
	glTexCoord2f(1,0);
	glVertex3f( rr,-rr,0);
	glTexCoord2f(1,1);
	glVertex3f( rr, rr,0);
	glTexCoord2f(0,1);
	glVertex3f(-rr, rr,0);
	glEnd();

	
	
	glPopMatrix();

}


void CPlayer::Update(float timeStep,CTileMap *map)
{
	int x,y,tx,ty,ir;


	if (bDead) targetRadius=0;

	phys.radius=phys.radius*0.85f + targetRadius*0.15f;

	if (bDead) return;

	ir=2+phys.radius;
	tx=phys.vPos.x;
	ty=phys.vPos.y;



	if (phys.bContact)
	{
		int tx=phys.ctx;
		int ty=phys.cty;

		int t=map->GetTile(tx,ty);

		if ((t==9 && phys.radius>0.7f) || (t==8 && bChilli))
		{
			map->SetTile(tx,ty,0);
			
			if (t==8)
				ENTITY_addFragments(tx+0.5f,ty+0.5f,1);
			else
				ENTITY_addFragments(tx+0.5f,ty+0.5f,0);

			// Rebuild the edge list, removing any from this tile

			CEdge *e=map->pEdges[tx+(ty*map->Width)];

			map->pEdges[tx+(ty*map->Width)]=NULL;

			while(e)
			{
				CEdge *nxt=e->pNext;

				
				if (e->t!=t)
				{
					e->pNext=map->pEdges[tx+(ty*map->Width)];
					map->pEdges[tx+(ty*map->Width)]=e;
				}
				else 
				{				
						delete e;
				}

				e=nxt;
			}


		}
	}



	float circ=3.142f*2.0f*phys.radius;

	angle+=(360.0f*phys.vVel.x/circ);

	
	float step;
	if (phys.airborne) step=0.003f;
				  else step=0.006f;


	int t=map->GetTile((phys.vPos.x),(phys.vPos.y));

	if (t==17 || t==18)
	{
		// Water

		if (KEY_state(KEY_UP))
		{
			phys.vVel.y=-(0.05f+ (phys.radius*0.05));			
		}


		if (t==18)
			phys.vVel.y-=0.003f;		// Bouyancy
		else
			phys.vVel.y-=0.0015f;	

		if (bChilli)
		{
			game->ShowMessage("Water puts out fire!");
		}

		bChilli=false;				// Flames out...

		phys.vVel.x*=0.99f;			// Water resistance
		phys.vVel.y*=0.99f;
	}
	else
	{
		if (bWings)
			phys.vVel.y+=0.0025f;		// Gravity
		else
			phys.vVel.y+=0.005f;		// Gravity
	}

	

	// Movement
	if (KEY_state(KEY_LEFT )) phys.vVel.x-=step;
	if (KEY_state(KEY_RIGHT)) phys.vVel.x+=step;
	
	// Jumping
	if (KEY_state(KEY_UP) && phys.airborne==0)
	{
		phys.vVel.y=-(0.1f+ (phys.radius*0.1));
		
		SFX_play2(3,55000.0f-(phys.radius*10000.0f));				
	}

	


	if (!phys.airborne)
	{
		phys.vVel.x*=0.98f;
		phys.vVel.y*=0.98f;
	}
	else
	{
		phys.vVel.x*=0.992f;
		phys.vVel.y*=0.992f;
	}

	phys.ClearCollLists();
	for(y=ty-ir;y<ty+ir;y++)
		for(x=tx-ir;x<tx+ir;x++)
		{
			CEdge *pE=map->GetEdgeList(x,y);

			if (pE) 
				phys.AddCollList(pE);
		}


	phys.Update(timeStep);


	if (scoreAdd>0)
	{
		scoreAdd--;
		score++;
	}
}