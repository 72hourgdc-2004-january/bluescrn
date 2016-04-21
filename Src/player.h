

// -----------------------------------------------------------
//
// 72hr GDC. Bluescrn's entry. still with no name.
//
// Player.cpp - Control system, special abilities, etc
//
// -----------------------------------------------------------


#ifndef _PLAYER_H_
#define _PLAYER_H_

class CPlayer
{

public:
	CPlayer();

	CPhysCircle phys;

	float		angle;
	float       xshift,yshift;

	void Update(float timeStep,CTileMap *map);
	void Render(void);
	void Reset(void);

	float		targetRadius;

	void GetPickup(int type);

	int  score;
	int  scoreAdd;

	bool bChilli;
	bool bWings;
	bool bDead;

	int  chocsLeft;
};


#endif