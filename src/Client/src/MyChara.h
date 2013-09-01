#pragma once

#include "Network.h"

class CMyChara
{
public:
	CMyChara( void );
	virtual ~CMyChara( void );

public:
	void loadFiles( void ) throw( int );
	void init( CNetwork *_Network );
	void move( void );
	void move2( void );

	float getX( void ){ return (charalist.front().x); }
	float getY( void ){ return (charalist.front().y); }
	list<CharaData> *getCharaList(){ return( &charalist ); }

private:
	list<CharaData> charalist;
	CAnimDraw CharaAnim;

public:
	CNetwork *pNetwork;

};
