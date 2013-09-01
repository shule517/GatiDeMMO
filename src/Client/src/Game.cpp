
#include "StdAfx.h"
#include "Game.h"

/* ========================================================================= */
/* 関数名：CGame::CGame														 */
/* 内容：コンストラクタ														 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
CGame::CGame( void )
{
}

/* ========================================================================= */
/* 関数名：CGame::~CGame													 */
/* 内容：デストラクタ														 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
CGame::~CGame( void )
{
}

/* ========================================================================= */
/* 関数名：CGame::loadFiles													 */
/* 内容：ファイルの読み込み													 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
void CGame::loadFiles( void ) throw(int)
{
	EffectCnt.loadFiles();
	MapCnt.loadFiles();
	PlayerChara.loadFiles();
	MyChara.loadFiles();
}

/* ========================================================================= */
/* 関数名：CGame::initStage													 */
/* 内容：ステージの読み込み													 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
void CGame::initStage( void )
{
	EffectCnt.init();
	MapCnt.init();
	PlayerChara.init();
	MyChara.init( &Network );

	Network.start( MyChara.getCharaList(), PlayerChara.getPlayerList(), EffectCnt.getEffectList() ); // ネット接続
}

/* ========================================================================= */
/* 関数名：CGame::drawGameMain												 */
/* 内容：ゲームメイン														 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
void CGame::drawGameMain( void )
{
	MapCnt.move( MyChara.getX(), MyChara.getY() );
	PlayerChara.move( MyChara.getX(), MyChara.getY() );
	MyChara.move();
	EffectCnt.move( MyChara.getX(), MyChara.getY() );
}
