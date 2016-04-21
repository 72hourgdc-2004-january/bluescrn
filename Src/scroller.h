

// -----------------------------------------------------------
//
// 72hr GDC. Bluescrn's entry. still with no name.
//
// Scroller.cpp - The serious stuff - Maps, collision edges
//
// -----------------------------------------------------------

#ifndef _SCROLLER_H_
#define _SCROLLER_H_

class CPlayer;


class CTileMap
{
public:

	
	void	  LoadMap(char *filename,CPlayer *plr);
	void      Render (float cx,float cy,float zoom,float look);

	CEdge    *GetEdgeList(int tx,int ty);			// For collision detection
	int		  GetTile(int tx,int ty);

	void      SetTile(int tx,int ty,int t);
private:
	void      AddEdge(int tx,int ty,float x0,float y0,float x1,float y1,int flags, float i,int EdgeTex,int t);
	

	float     CentreX,CentreY;
	float     Zoom;

	int       TileTexID;
	

public:
	int		 *pMap;						// The tiles  
	CEdge   **pEdges;					// Tile edges (one list per map cell)
	
	int		  Width;
	int		  Height;
	
};



#endif