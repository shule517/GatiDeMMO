#pragma once

class CGameFrame
{
public:
	CGameFrame( void );
	virtual ~CGameFrame( void );

private:
	virtual void loadFiles( void ) throw( int ) {}
	virtual void initStage( void ) {}
//	int starttime;
	static unsigned int animcounter;
	static float idouryou[20];
	void setIdouHosei();

protected:
	virtual void drawGameMain( void );

public:
	void doMainLoop( HINSTANCE hInst, int nCmdShow );
	static unsigned int getAnimCnt();
	static float IdouHosei( int num );

};
