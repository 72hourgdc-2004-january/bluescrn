
// -----------------------------------------------------------
//
// 72hr GDC. Bluescrn's entry. still with no name.
//
// Collision.cpp - Circle-Line Segment collision detection/response
//
// -----------------------------------------------------------

#include <math.h>

#include "game.h"


// reflect a vector about another vector
// (usually an edge normal)
// -----------------------------------------------

void reflect(CVec2D *v,CVec2D *norm)
{
	CVec2D vL,normN;
	float d;

	// Normalize both
	float l=sqrt(v->x*v->x+v->y*v->y);
	vL.x=v->x/l;
	vL.y=v->y/l;

	float l2=sqrt(norm->x*norm->x+norm->y*norm->y);	
	normN.x=norm->x/l2;
	normN.y=norm->y/l2;

	vL.x=-vL.x;
	vL.y=-vL.y;

	d=2*(normN.x*vL.x+normN.y*vL.y);

	v->y= v->y + (normN.y*d*l);
	v->x= v->x + (normN.x*d*l);	
}





bool CEdge::CollideCircle(float px,float py,float radius,CVec2D *vNormal,float *depth)
{
	float r=(p[1].x-p[0].x)*(p[0].y-py)
		   -(p[1].y-p[0].y)*(p[0].x-px);

	float i=(p[1].x-p[0].x)*(p[1].x-p[0].x)
		   +(p[1].y-p[0].y)*(p[1].y-p[0].y);

	float l=sqrt(i);	
	l=r/l;


	if (l<=radius && l>=0)
	{
		float t,cx,cy;
		CVec2D normal;

		// Calculate normalized normal to the edge
		normal.x=  p[1].y-p[0].y;
		normal.y=-(p[1].x-p[0].x);
		float nl=(normal.x*normal.x)+(normal.y*normal.y);
		nl=sqrt(nl);
		normal.x=normal.x/nl;
		normal.y=normal.y/nl;			
			
		cx=px-(normal.x*l);
		cy=py-(normal.y*l);

		// Eliminate collisions that are off the ends of the segment
		if (fabs(p[1].x-p[0].x)>fabs(p[1].y-p[0].y))
		{	
			// Find T, use X component
			t= (cx-p[0].x)/(p[1].x-p[0].x);
		}
		else
		{
			// Find T, use Y component
			t= (cy-p[0].y)/(p[1].y-p[0].y);
		}

		if (t<0 || t>1.0f) return false;

		vNormal->x=normal.x;
		vNormal->y=normal.y;
		*depth=-(l-radius);

		return true;
	}
	else
	{
		return false;
	}
}




void CPhysCircle::Update(float timestep)
{
	bool hitSomething=false;
	bool bOnGround=false;
	int n;
	CEdge *edge;

	vPos.x+=vVel.x;		// multiply by timestep?...
	vPos.y+=vVel.y;

	// Have we hit anything?

	bContact=false;

	// Check for line collisions first
	// ------------------------------------

	for(int pass=0;pass<2;pass++)
	{
	  if (!hitSomething)	  
		for(n=0;n<numEdgeLists;n++)
		{
			edge=pEdgeLists[n];

			while(edge)
			{
				float  depth;
				CVec2D normal;
				bool   bHit=false;

				if (pass==0)
				{
					// First pass - collide with the line segment
					bHit=edge->CollideCircle(vPos.x,vPos.y,radius,&normal,&depth);
				}					
				else
				{
				
					// Second pass - collide with the endpoints					
					int i=0;
					float dx,dy,dx2,dy2;
					
					dx=(edge->p[0].x-vPos.x);
					dy=(edge->p[0].y-vPos.y);
					dx2=dx*dx;
					dy2=dy*dy;

					float l=sqrt(dx2+dy2);

					if (l<radius && l>0)	// 0.1f
					{
						bHit=true;
						depth=radius-l;
						normal.x=-dx/l;
						normal.y=-dy/l;
					}
				
				}

				if (bHit)			
				{		
					extern CTileMap map;
					
					int t;
					t=map.GetTile(edge->p[0].x,edge->p[0].y);

					if (t>=8 && t<16)
					{
						bContact=true;
						ctx=edge->p[0].x;
						cty=edge->p[0].y;
					}


					if (normal.y<-0.5f)
					{
						bOnGround=true;
						if (airborne>0) airborne--;					
					}

					// Push the sphere back to a non-penetrating position
					vPos.x+=normal.x*depth;
					vPos.y+=normal.y*depth;

					
					// Reaction force?
					CVec2D vNormVel;
					
					float speed=sqrt((vVel.x*vVel.x)+(vVel.y*vVel.y)); 
					
					if (speed>0)
					{
						vNormVel.x=vVel.x/speed;
						vNormVel.y=vVel.y/speed;

						float c=(-vNormVel.x*normal.x)+(-vNormVel.y*normal.y);  // dot product
						
						if (c>0)
						{
							float i;

							//if (speed>512) i=175; else 
							i=1.0f-bounce;
						//	if (airborne && speed>0.05f) i=0.5f;
							
							{								
								reflect(&vVel,&normal);
								vVel.x=vVel.x*(1.0f-i);
								vVel.y=vVel.y*(1.0f-i);
																	   
								vVel.x+=((vNormVel.x+(c*normal.x))*speed*i);
								vVel.y+=((vNormVel.y+(c*normal.y))*speed*i);
							}

						}

		
					}								
				}
				
				edge=edge->pNext;
			}		
		}
	}

	if (!bOnGround) airborne=3;
}



void CPhysCircle::ClearCollLists(void)
{
	numEdgeLists=0;
}


void CPhysCircle::AddCollList(CEdge *pEdge)
{
	pEdgeLists[numEdgeLists]=pEdge;
	numEdgeLists++;
}