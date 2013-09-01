#pragma once

#include "Network.h"
#include "MyChara.h"
#include "PlayerChara.h"
#include "MapControl.h"
#include "EffectControl.h"

class CGame : public CGameFrame
{
public:
	CGame( void );
	virtual ~CGame( void );

public:
	CNetwork		Network;
	CMyChara		MyChara;
	CPlayerChara	PlayerChara;
	CMapControl		MapCnt;
	CEffectControl	EffectCnt;

public:
	void loadFiles( void ) throw(int);
	void initStage( void );
	void drawGameMain( void );

};
