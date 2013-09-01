#include "StdAfx.h"
#include "MapControl.h"

/* ========================================================================= */
/* 関数名：CMapControl::CMapControl											 */
/* 内容：コンストラクタ														 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
CMapControl::CMapControl( void )
{
}

/* ========================================================================= */
/* 関数名：CMapControl::~CMapControl										 */
/* 内容：デストラクタ														 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
CMapControl::~CMapControl( void )
{
}

/* ========================================================================= */
/* 関数名：CMapControl::loadFiles											 */
/* 内容：ファイルの読み込み													 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
void CMapControl::loadFiles( void )
{
	int ghandle[216];
	if( LoadDivGraph( "data/glasstown001.bmp", 216, 8, 27, 32, 32, ghandle ) == -1 ) throw( -2 );

	for( int i = 0; i < 216; i++ )
		MapAnim.setImage( i, ghandle[i] );
}

/* ========================================================================= */
/* 関数名：CMapControl::init												 */
/* 内容：初期化																 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
void CMapControl::init()
{
	for( int y = 0; y < MAP_HEIGHT; y++ )
	{
		for( int x = 0; x < MAP_WIDTH; x++ )
		{
			switch( rand()%3 )
			{
			case 0:
				mapdat[y][x] = 0;
				break;
			case 1:
				mapdat[y][x] = 1;
				break;
			case 2:
				mapdat[y][x] = 2;
				break;
			}
		}
	}
}

/* ========================================================================= */
/* 関数名：CMapControl::move												 */
/* 内容：マップの処理														 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
void CMapControl::move( float JikiX, float JikiY )
{
	for( int y = 0; y < MAP_HEIGHT; y++ )
	{
		for( int x = 0; x < MAP_WIDTH; x++ )
		{
			if( ( JikiX - WIDTH/2 - 32 < x*32 && x*32 < JikiX + WIDTH/2 )  && ( JikiY - HEIGHT/2 - 32 < y*32 && y*32 < JikiY + HEIGHT/2 ) )
				MapAnim.draw( mapdat[y][x], x*32 - JikiX + WIDTH/2, y*32 - JikiY + HEIGHT/2 );
		}
	}
}
