

// -----------------------------------------------------------
//
// 72hr GDC. Bluescrn's entry. 'Size Matters'
//
// Entities.h - Pickups and Enemies
//
// -----------------------------------------------------------



#define MAX_ENTITIES 128

enum
{
	ENTITY_CHOC=1,
	ENTITY_CHILLI,
	ENTITY_NRG,
	ENTITY_LAGER,
	ENTITY_SPIKES,
	ENTITY_WATER,
	ENTITY_WATERSURF,
	ENTITY_AIRLOCK
};

class CEntity
{
public:

	virtual void Init(int tx,int ty,int type);
	virtual bool Update(void);
	virtual void Render(int layer);
	virtual void Collide(CPlayer *p);
	
	int	   state;
	int    type;
	float  timer;

	float  radius;
	CVec2D vPos;
	CVec2D vVel;
};



class CPickup:public CEntity
{
	void Collide(CPlayer *plr);
	bool Update();
	void Render(int layer);
};

class CSpike:public CEntity
{
	void Collide(CPlayer *plr);	
	void Render(int layer);
};

class CWater:public CEntity
{
//	void Collide(CPlayer *plr);		
	void Render(int layer);
};

class CAirlock:public CEntity
{
//	void Collide(CPlayer *plr);		
	void Render(int layer);
};



// A mini-particle-system
class CFragments:public CEntity
{
	CPhysCircle  frags[16];
	int          numFrags;
	
	float		 angles[16];
	float		 angvels[16];
	float		 lifes[16];
		
	virtual bool Update(void);
	virtual void Render(int layer);
public:
	void Init(float tx,float ty,int type,int flags);
};


void ENTITY_process(CPlayer *plr);
void ENTITY_init();
void ENTITY_reset();
void ENTITY_add(int type,int tx,int ty);
void ENTITY_addFragments(float x,float t,int type);

