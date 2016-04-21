
// -----------------------------------------------------------
//
// 72hr GDC. Bluescrn's entry. 'Size Matters'
//
// Entities.cpp - Pickups and Enemies
//
// -----------------------------------------------------------

#include <math.h>

#include "game.h"


enum
{
	STATE_INITIAL=0,
	STATE_EATEN
};


CEntity *entities[MAX_ENTITIES];

void ENTITY_init()
{
	for (int i=0;i<MAX_ENTITIES;i++)
	{
		entities[i]=0;
	}
}


void ENTITY_reset()
{
	for (int i=0;i<MAX_ENTITIES;i++)
	{
		if (entities[i])
		{
			delete entities[i];
			entities[i]=NULL;
		}
	}
}






int ENTITY_get()
{
	int i;

	for(i=0;i<MAX_ENTITIES;i++)
	{
		if (entities[i]==NULL) return i;
	}

	return NULL;

}

void ENTITY_add(int type,int tx,int ty)
{
	int i;
	CEntity *e;

	switch (type)
	{
		case ENTITY_CHOC:			
		case ENTITY_CHILLI:
		case ENTITY_NRG:
		case ENTITY_LAGER:				
				
				i=ENTITY_get();
				if (e)
				{
					e=new CPickup;
					e->Init(tx,ty,type);
					entities[i]=e;
				}
			break;


		case ENTITY_SPIKES:
				
				i=ENTITY_get();
				if (e)
				{
					e=new CSpike;
					e->Init(tx,ty,type);
					entities[i]=e;
				}
			break;

		case ENTITY_WATER:
		case ENTITY_WATERSURF:
			    i=ENTITY_get();
				if (e)
				{
					e=new CWater;
					e->Init(tx,ty,type);
					entities[i]=e;
				}
			break;


				
		case ENTITY_AIRLOCK:
			    i=ENTITY_get();
				if (e)
				{
					e=new CAirlock;
					e->Init(tx,ty,type);
					entities[i]=e;
				}
			break;
	}
}

void ENTITY_addFragments(float x,float y,int type)
{
	CFragments *e;
	int i=ENTITY_get();
	if (e)
	{
		e=new CFragments;
		e->Init(x,y,type,0);
		entities[i]=e;
	}
}

void ENTITY_process(CPlayer *plr)
{
	int i,j;
	
	glDepthMask(GL_FALSE);

	for(i=0;i<MAX_ENTITIES;i++)
	{
		CEntity *e;
		e=entities[i];
		if (e) 
		{
			if (e->Update())
			{
				e->Collide(plr);
				e->Render(0);
			}
			else
			{
				delete entities[i];
				entities[i]=NULL;
			}
		}
	}

	plr->Render();

	for(j=1;j<=2;j++)
		for(i=0;i<MAX_ENTITIES;i++)
		{
			CEntity *e;
			e=entities[i];
			if (e) e->Render(j);
		}

	glDepthMask(GL_TRUE);
}

void CEntity::Collide(CPlayer *p) {}
void CEntity::Render(int layer)  {}
bool CEntity::Update(void) {return true;}

void CEntity::Init(int tx,int ty,int type)
{
	CEntity *e;
	this->type=type;
	vPos.x=((float)(tx))+0.5f;
	vPos.y=((float)(ty))+0.5f;
	vVel.x=vVel.y=0;
	state=STATE_INITIAL;
	timer=0;
}




void CPickup::Render(int layer)
{
	float angle=0;
	float r;

	

	if (state==STATE_INITIAL)
	{
		radius=0.45f;
	}

	if (layer==0)
	{	
		r=radius*1.8f;

		angle=g_bounceTimer*15.0f;

		glDisable(GL_DEPTH_TEST);

		for(int p=0;p<3;p++)
		{
			angle=-angle*0.7f;

			glPushMatrix();
			glTranslatef(vPos.x,vPos.y,-0.5f);	

			float sc=1.0f+(0.3f*sin(((float)(p))+g_bounceTimer*1.0f));
			glScalef(sc,sc,sc);

			glRotatef(angle,0,0,1);

			switch(type)
			{
	
				case ENTITY_CHILLI:
					GL_bindTexture(11);
					glColor3f(0.6f,0.2f,0.1f);
					break;

				case ENTITY_NRG:
					GL_bindTexture(10);
					glColor3f(0.3f,0.3f,1.0f);
					break;

				default:
					GL_bindTexture(10);
					if      (p==0) glColor3f(0.6f,0.6f,0);
					else if (p==1) glColor3f(0,0.6f,0.6f);
						  	  else glColor3f(0.6f,0,0.6f);
			}

			
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
		glEnable(GL_DEPTH_TEST);
	}


	if (layer==1)
	{
		r=radius;
		
		angle=sin(vPos.x+g_bounceTimer)*45.0f;

		glPushMatrix();
			glTranslatef(vPos.x,vPos.y,-0.5f);	

			glRotatef(angle,0,0,1);

			GL_bindTexture(19+type);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			glColor3f(1,1,1);

			glBegin(GL_POLYGON);
			glTexCoord2f(0,0);
			glVertex3f(-r, r,0);
			glTexCoord2f(1,0);
			glVertex3f( r, r,0);
			glTexCoord2f(1,1);
			glVertex3f( r,-r,0);
			glTexCoord2f(0,1);
			glVertex3f(-r,-r,0);
			glEnd();
		glPopMatrix();
	}
}

void CPickup::Collide(CPlayer *plr)
{
	float r2;

	if (state!=STATE_INITIAL) return;

	r2=(vPos.x-plr->phys.vPos.x)*(vPos.x-plr->phys.vPos.x) +
	   (vPos.y-plr->phys.vPos.y)*(vPos.y-plr->phys.vPos.y);

	if (r2< (plr->phys.radius+0.3))
	{
		state=STATE_EATEN;
		plr->GetPickup(type);
	}
}


bool CPickup::Update()
{	
	if (state==STATE_EATEN)
	{
		radius-=0.025f;

		if (radius<0) return false;
	}
	
	return true;
}







// Spikes
// ------------------------

void CSpike::Collide(CPlayer *plr)
{
	float r2;

	r2=(vPos.x-plr->phys.vPos.x)*(vPos.x-plr->phys.vPos.x) +
	   (vPos.y-plr->phys.vPos.y)*(vPos.y-plr->phys.vPos.y);

	if (r2< (plr->phys.radius+0.3))
	{		
		plr->GetPickup(ENTITY_SPIKES);
	}
}



void CSpike::Render(int layer)
{
	float angle=0;
	float u,r;


	//if (layer==1)
	{
		if (layer==1) 
		{
			u=1.0f;
			r=0.5f;
		}
		else
		{
			u=-1.0f;
			r=0.5f;
		}

		float i=0.3f+(layer*0.33f);
		
		angle=0; //sin(vPos.x+g_bounceTimer)*45.0f;

		angle=(layer-1)*5;

		glPushMatrix();
			glTranslatef(vPos.x,vPos.y,-0.9f + ((float)(layer))*0.4f );	

			glRotatef(-angle,1,0,0);

			GL_bindTexture(32);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			glColor3f(i,i,i);

			glBegin(GL_POLYGON);
			glTexCoord2f(0,0);
			glVertex3f(-r, r,0);
			glTexCoord2f(u,0);
			glVertex3f( r, r,0);
			glTexCoord2f(u,1);
			glVertex3f( r,-r,0);
			glTexCoord2f(0,1);
			glVertex3f(-r,-r,0);
			glEnd();
		glPopMatrix();
	}
}




void CWater::Render(int layer)
{
	float angle=0;
	float u,v,u1,v1,r;

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	u=g_bounceTimer*0.03f;
	v=g_bounceTimer*0.05f;

	//if (layer==1)
	{
		if (layer==1) 
		{			
			r=0.5f;
			v=-v;
		}
		else
		{
			u=-u;
			r=0.5f;
		}

		u1=u+1;
		v1=v+1;

		if (layer==0) 
		{
			u*=2;
			u1*=2;
		}

		if (layer==1) 
		{
			v*=2;
			v1*=2;
		}


		float i=0.3f+(layer*0.33f);
		
		angle=0; //sin(vPos.x+g_bounceTimer)*45.0f;

		angle=(layer-1)*5;

		glPushMatrix();
			glTranslatef(vPos.x,vPos.y,-0.8f + ((float)(layer))*0.3f );	

			//glRotatef(-angle,1,0,0);

			GL_bindTexture(64);
			
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);

			glColor4f(1,1,1,0.2f);

			glBegin(GL_POLYGON);
			glTexCoord2f(u1,v1);
			glVertex3f(-r, r,0);
			glTexCoord2f(u,v1);
			glVertex3f( r, r,0);
			glTexCoord2f(u,v);
			glVertex3f( r,-r,0);
			glTexCoord2f(u1,v);
			glVertex3f(-r,-r,0);
			glEnd();
		glPopMatrix();
	}

	glDepthMask(GL_FALSE);
}



void CAirlock::Render(int layer)
{	
	float r=0.5f;
	float aa,bb;

	aa=1.0f/64.0f;
	bb=63.0f/64.0f;

	glDepthMask(GL_FALSE);


	//if (layer==0 || layer==2)
	{			
		glPushMatrix();
			glTranslatef(vPos.x,vPos.y,-0.8f + ((float)(layer))*0.3f );	

			GL_bindTexture(65);
			
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);

			glColor4f(1,1,1,1);

			glBegin(GL_POLYGON);
			glTexCoord2f(aa,bb);
			glVertex3f(-r, r,0);
			glTexCoord2f(bb,bb);
			glVertex3f( r, r,0);
			glTexCoord2f(bb,aa);
			glVertex3f( r,-r,0);
			glTexCoord2f(aa,aa);
			glVertex3f(-r,-r,0);
			glEnd();
		glPopMatrix();
	}

	glDepthMask(GL_FALSE);
}









/*
class CFragments:public CEntity
{
	cPhysCircle  frags[16];
	int          numFrags;

	virtual bool Update(void);
	virtual void Render(int layer);
}
*/

extern CTileMap map;
extern CPlayer player;

void CFragments::Init(float x,float  y,int type,int flags)
{
	int n;

	numFrags=16;

	float ang=-0.5f;
	float p=0.03f;

	this->type=type;

	for(n=0;n<16;n++)
	{
		frags[n].radius=0.25f;
		
		if (type==2)
		{
			ang+=1;
			float r=n;
			r*=(player.phys.radius*(1.0f/16.0f));
			frags[n].vPos.x=x+r*sin(ang);
			frags[n].vPos.y=y+r*cos(ang);			
			frags[n].vVel.x=0.2f*r*sin(ang);
			frags[n].vVel.y=0.2f*r*cos(ang);
			frags[n].bounce=0.2f;
		}
		else
		{
			frags[n].vPos.x=x;
			frags[n].vPos.y=y;

			if ((n&3)==0)
			{
				ang=-0.5f;
				p+=0.03f;
			}

			frags[n].vPos.x=x+  (((float)(n&3))-1.5f)*0.25f;			
			frags[n].vPos.y=y+  (((float)(n>>2))-1.5f)*0.25f;
			
			
			
			if (type==1)
			{
				frags[n].vVel.x=-p*sin(ang);
				frags[n].vVel.y=0.2f*-p*cos(ang);
				frags[n].bounce=0.1f;
			}
			else
			{		
				frags[n].vVel.x=-p*sin(ang);
				frags[n].vVel.y=-p*cos(ang);
				frags[n].bounce=0.4f;
			}
		}
		
		angles[n]=rand()%360;
		angvels[n]=8.0f;
		lifes[n]=1.0f;

		ang+=1.0f/4.0f;
	}

}



bool CFragments::Update(void)
{
	int x,y;
	int n;
	bool bLive=false;


	for(n=0;n<numFrags;n++)
	{
		CPhysCircle *phys=&frags[n];

		int ir=2;
		int tx=phys->vPos.x;
		int ty=phys->vPos.y;
		

		phys->ClearCollLists();
		for(y=ty-ir;y<ty+ir;y++)
			for(x=tx-ir;x<tx+ir;x++)
			{
				CEdge *pE=map.GetEdgeList(x,y);

				if (pE) 
					phys->AddCollList(pE);
			}

		if (type==0 || type==2)
			phys->vVel.y+=0.005f;	// block fragments
		else
			phys->vVel.y+=0.002f;   // flames


		phys->Update(1.0f/60.0f);

		angles[n]+=angvels[n];
		lifes[n]-=0.01f;

		if (lifes[n]<0)
		   lifes[n]=0;
		else
		   bLive=true;
	}


	

	return bLive;
}




void CFragments::Render(int layer)
{	
	float r=0.25f;
	float aa,bb;

	aa=1.0f/64.0f;
	bb=63.0f/64.0f;

	glDepthMask(GL_FALSE);

	int n;

	
	if (type==1) r=0.35f;
	if (type!=0) r=r*lifes[0];

	if (layer==1)
	{			
		for(n=0;n<numFrags;n++)
		{
		glPushMatrix();
			glTranslatef(frags[n].vPos.x,frags[n].vPos.y,-0.5f );	

			glRotatef(angles[n],0,0,1);

			GL_bindTexture(40+type);
			
			glEnable(GL_BLEND);

			if (type==0 || type==2)
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			else
				glBlendFunc(GL_SRC_ALPHA,GL_ONE);
			

			

			

			//glColor3f(1,1,1);
			glColor4f(1,1,1,lifes[n]);

			glBegin(GL_POLYGON);
			glTexCoord2f(aa,bb);
			glVertex3f(-r, r,0);
			glTexCoord2f(bb,bb);
			glVertex3f( r, r,0);
			glTexCoord2f(bb,aa);
			glVertex3f( r,-r,0);
			glTexCoord2f(aa,aa);
			glVertex3f(-r,-r,0);
			glEnd();
		glPopMatrix();
		}
	}

	glDepthMask(GL_FALSE);
}


