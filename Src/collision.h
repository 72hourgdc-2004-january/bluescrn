

// -----------------------------------------------------------
//
// 72hr GDC. Bluescrn's entry. still with no name.
//
// Collision.cpp - Circle-Line Segment collision detection/response
//
// -----------------------------------------------------------


#ifndef _COLLISION_H_
#define _COLLISION_H_

class CVec2D
{
public:
	float x;
	float y;	
};




class CEdge
{
public:
	CVec2D        p[2];
	int			  flags;

	CEdge	     *pNext;	
	bool		  CollideCircle(float px,float py,float radius,CVec2D *normal,float *depth);

	float         i; // brightness for shading, 0-1

	int           tex;

	int           t;
};



#define MAX_LISTS 256

class CPhysCircle
{

public:

	CVec2D vPos;
	CVec2D vVel;	
	float  radius;
	float  bounce;
	int    airborne;

	bool   bContact;
	int    ctx,cty;		// Contact tile

	void   Update(float timestep);      // timestep??
	void   ClearCollLists(void);
	void   AddCollList(CEdge *pEdge);
private:

	int    numEdgeLists;
	CEdge *pEdgeLists[MAX_LISTS];
};


#endif