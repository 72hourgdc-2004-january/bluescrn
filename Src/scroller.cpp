

// -----------------------------------------------------------
//
// 72hr GDC. Bluescrn's entry. still with no name.
//
// Scroller.cpp - The serious stuff - Maps, collision edges
//
// -----------------------------------------------------------

#include "game.h"

#include "tiledefs.h"

// --------------------------------------------------------
// Load/Init a map and tileset
// --------------------------------------------------------

void  CTileMap::LoadMap(char *filename,CPlayer *plr)
{

	char fn2[512];
	int *pM;
	int wh[2];
	FILE *f;

	plr->chocsLeft=0;

	sprintf(fn2,"data\\%s",filename);

	f=fopen(fn2,"rb");
	fread(&wh,2,4,f);

	Width=wh[0];
	Height=wh[1];

	pMap=(int *)malloc(Width*Height*4);
	
	fread(pMap,1,4*Width*Height,f);

	fclose(f);
	
	pEdges=(CEdge **)malloc(Width*Height*sizeof(CEdge *));
	memset(pEdges,0,Width*Height*sizeof(CEdge *));
	

	


	int x,y;
	
	for(y=0;y<Height;y++)
		for(x=0;x<Width;x++)
		{
			int t=pMap[x+Width*y];

			if (t<16)
				SetTile(x,y,t);
			else
			{	
				SetTile(x,y,0);
				switch (t)
				{
					case 32:
						// Player start pos
							plr->phys.vPos.x=((float)(x))+0.5f;
							plr->phys.vPos.y=((float)(y))+0.5f;
						break;

					case 24:
						ENTITY_add(ENTITY_CHILLI,x,y);
						break;

					case 25:
						ENTITY_add(ENTITY_NRG,x,y);
						break;

					case 26:
						ENTITY_add(ENTITY_CHOC,x,y);
						plr->chocsLeft++;
						break;

					case 27:
						ENTITY_add(ENTITY_LAGER,x,y);
						break;

					case 16:						
						ENTITY_add(ENTITY_SPIKES,x,y);
						break;

					case 17:
						SetTile(x,y,17);
						ENTITY_add(ENTITY_WATERSURF,x,y);
						break;

					case 18:
						SetTile(x,y,18);
						ENTITY_add(ENTITY_WATER,x,y);
						break;

					case 19:
						SetTile(x,y,19);
						ENTITY_add(ENTITY_AIRLOCK,x,y);
						break;
				}

			
			}
				
		}


}



// --------------------------------------------------------
// Place a tile on the map, and set up the edges
// --------------------------------------------------------

void CTileMap::SetTile(int tx,int ty,int t)
{
	int ts,e;

	pMap[tx+Width*ty]=t;
	ts=TileShape[t];

	

	// Now add the edges
	for(e=0;e<6;e++)
	{
		if (EdgeDefs[ts][e][4]==0)
		{
	   
		AddEdge(tx,ty,	EdgeDefs[ts][e][0],
						EdgeDefs[ts][e][1],
						EdgeDefs[ts][e][2],
						EdgeDefs[ts][e][3],0,EdgeDefs[ts][e][5],TileEdge[t],t);
		}
	}
}





void CTileMap::AddEdge(int tx,int ty,float x0,float y0,float x1,float y1,int flags,float i,int edgeTex,int t)
{
	float minx,miny;
	int   ctx,cty;

	CEdge *pE;
	CEdge **pPrevPtr;


	// Get min X and Y, pick list, convert coords relative->absolute
	if (x0<x1) minx=x0; else minx=x1;
	if (y0<y1) miny=y0; else miny=y1;

	ctx=(int)((float)(minx+((float)(tx))));
	cty=(int)((float)(miny+((float)(ty))));

	// Convert to world coords (from tile-local)
	x0+=((float)(tx));
	x1+=((float)(tx));
	y0+=((float)(ty));
	y1+=((float)(ty));

	// Check tile is in range
	if (ctx<0 || ctx>=Width)  return;
	if (cty<0 || cty>=Height) return;


	// Check for an opposite line, if found, remove+return
	pPrevPtr=&pEdges[ctx+(cty*Width)];
	pE=pEdges[ctx+(cty*Width)];


	while (pE)
	{
		if (pE->p[0].x==x1 && pE->p[1].x==x0 &&
			pE->p[0].y==y1 && pE->p[1].y==y0)
		{
			*pPrevPtr=pE->pNext;
			delete(pE);
			return;
		}

		pPrevPtr=&pE->pNext;
		pE=pE->pNext;
	}
	

	// Else add the new segment

	pE=new CEdge;
	pE->pNext=pEdges[ctx+(cty*Width)];
	pEdges[ctx+(cty*Width)]=pE;
	pE->p[0].x=x0;  pE->p[1].x=x1;
	pE->p[0].y=y0;  pE->p[1].y=y1;
	pE->flags=flags;

	pE->tex=edgeTex;
	pE->i=i;
	pE->t=t;
}





// --------------------------------------------------------
// Render the map
// --------------------------------------------------------

void  CTileMap::Render (float cx,float cy,float zoom,float look)
{
	int x,y;
	float z,fx,fy;
	float ou,ov,u0,v0,tw,th;

	int i,tx;
	float px,py;


	glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);		
	
	


	th=tw=(1.0f/4.0f)-(2.0f/128.0f);		// tile width and height on texture
	ou=ov=(1.0f/128.0f);

	glColor3f(0.9f,0.9f,0.9f);
	glLoadIdentity();		
	glTranslatef(0,0,-zoom);	
	glScalef(1,-1,1);
	glRotatef(look,0,0,1);
	
	glTranslatef(-cx,-cy+1.5f,0);	


	float sc=0.25f;
	float xo,yo;

	xo=-0.5f*cx*sc;
	yo=-0.5f*cy*sc;

	GL_bindTexture(17);						// back layer

	glBegin(GL_POLYGON);
	glTexCoord2f(xo,yo);
	glVertex3f(0,0,-1);
	glTexCoord2f(xo+(Width*sc),yo);
	glVertex3f(Width,0,-1);
	glTexCoord2f(xo+(Width*sc),yo+(Height*sc));
	glVertex3f(Width,Height,-1);
	glTexCoord2f(xo,yo+(Height*sc));
	glVertex3f(0,Height,-1);
	glEnd();

	GL_bindTexture(16);						// tileset


	for(y=0;y<Height;y++)
	{
		fy=y;

		for(x=0;x<Width;x++)
		{
			int t=pMap[x+(y*Width)];
			int ts=TileShape[t];
	
			

			if (t)
			{
				fx=x;
				i=0;

				tx=TileTex[t];
				u0=ou+ ((float)(tx&3))*(0.25f);
				v0=ov+ ((float)(tx>>2))*(0.25f);
				

				glBegin(GL_POLYGON);
				
				while (EdgeDefs[ts][i][4]>=0.0f)
				{
					px=EdgeDefs[ts][i][0];
					py=EdgeDefs[ts][i][1];
					glTexCoord2f(u0+(tw*px),v0+(th*py));
					glVertex3f(fx+px		,fy+py	,0);
					i++;
				}
				
				glEnd();
			}
		}
	}



	// Render the edges
	// ------------------------------------------

//	glDisable(GL_DEPTH_TEST);		
//	GL_bindTexture(0);


	
	glColor3f(0.5f,0.5f,0.5f);
	for(y=0;y<Height;y++)
		for(x=0;x<Width;x++)
		{
			CEdge *e=pEdges[x+y*Width];
		
			

			while(e)
			{
				tx=e->tex;
				u0=ou+ ((float)(tx&3))*(0.25f);
				v0=ov+ ((float)(tx>>2))*(0.25f);


				glBegin(GL_POLYGON);
				
				glColor3f(e->i,e->i,e->i);

				glTexCoord2f(u0+(tw*0.2f),v0+(th*0));
				glVertex3f(e->p[0].x		,e->p[0].y			,0);
				glTexCoord2f(u0+(tw*0.7f),v0+(th*0));
				glVertex3f(e->p[1].x		,e->p[1].y			,0);

				glTexCoord2f(u0+(tw*0.7f),v0+(th*1));
				glVertex3f(e->p[1].x		,e->p[1].y			,-1.0f);
				glTexCoord2f(u0+(tw*0.2f),v0+(th*1));
				glVertex3f(e->p[0].x		,e->p[0].y			,-1.0f);
				glEnd();

				e=e->pNext;
			}		
		}



	/*
	glDisable(GL_DEPTH_TEST);		
	GL_bindTexture(0);
	glColor3f(0,1,0);
	for(y=0;y<Height;y++)
		for(x=0;x<Width;x++)
		{
			CEdge *e=pEdges[x+y*Width];

			while(e)
			{
				glBegin(GL_LINES);
				glVertex3f(e->p[0].x		,e->p[0].y			,0);
				glVertex3f(e->p[1].x		,e->p[1].y			,0);
				glEnd();

				e=e->pNext;
			}		
		}
	*/
}




CEdge    *CTileMap::GetEdgeList(int tx,int ty)			// For collision detection
{
	if (tx<0 || tx>=Width)  return NULL;
	if (ty<0 || ty>=Height) return NULL;

	return pEdges[tx+(ty*Width)];
}


int		  CTileMap::GetTile(int tx,int ty)
{
	if (tx<0 || tx>=Width)  return 0;
	if (ty<0 || ty>=Height) return 0;

	return pMap[tx+(ty*Width)];
}

