#pragma once

class CPlayerChara
{
public:
	CPlayerChara( void );
	virtual ~CPlayerChara( void );

public:
	void init( void );
	void loadFiles( void ) throw( int );
	void move( float JikiX, float JikiY );
	void move2( void );

	list<CharaData> *getPlayerList(){ return( &PlayerList ); }

private:
	CAnimDraw		PlayerAnim;
	list<CharaData> PlayerList;

};
