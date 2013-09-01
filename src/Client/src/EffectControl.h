#pragma once

class CEffectControl
{
public:
	CEffectControl(void);
	virtual ~CEffectControl(void);

public:
	void loadFiles( void ) throw( int );
	void init();// CNetwork *_Network );
	void move( float JikiX, float JikiY );

	void drawDamage( int x, int y, char *str, int alpha );

	/*
	float getX( void ){ return (charalist.front().x); }
	float getY( void ){ return (charalist.front().y); }
	*/
	list<CharaData> *getEffectList(){ return( &effectlist ); }

private:
	list<CharaData> effectlist;
	CAnimDraw EffectAnim;

public:
//	CNetwork *pNetwork;
};
