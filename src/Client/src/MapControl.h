#pragma once

const int MAP_WIDTH		= 100;
const int MAP_HEIGHT	= 100;

class CMapControl
{
public:
	CMapControl( void );
	virtual ~CMapControl( void );

public:
	void loadFiles( void ) throw( int );
	void init( void );
	void move( float JikiX, float JikiY );

private:
	CAnimDraw MapAnim;
	int mapdat[MAP_HEIGHT][MAP_WIDTH];

};
