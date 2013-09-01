#include "StdAfx.h"
#include "EffectControl.h"

/* ========================================================================= */
/* 関数名：CEffectControl::CEffectControl									 */
/* 内容：コンストラクタ														 */
/* 日付：2007/09/14															 */
/* ========================================================================= */
CEffectControl::CEffectControl( void )
{
}

/* ========================================================================= */
/* 関数名：CEffectControl::~CEffectControl									 */
/* 内容：デストラクタ														 */
/* 日付：2007/09/14															 */
/* ========================================================================= */
CEffectControl::~CEffectControl( void )
{
}

/* ========================================================================= */
/* 関数名：CEffectControl::init												 */
/* 内容：初期化																 */
/* 日付：2007/09/14															 */
/* ========================================================================= */
void CEffectControl::init()
{
	effectlist.clear();	//自キャラのリストを空にする
}

/* ========================================================================= */
/* 関数名：CEffectControl::loadFiles										 */
/* 内容：ファイルの読み込み													 */
/* 日付：2007/09/14															 */
/* ========================================================================= */
void CEffectControl::loadFiles()
{
	/*
	int ghandle[13];
	ghandle[0] = LoadGraph( "data/effect/lord_stick_a.bmp" );
	ghandle[1] = LoadGraph( "data/effect/lord_stick_b.bmp" );
	ghandle[2] = LoadGraph( "data/effect/lord_stick_c.bmp" );
	ghandle[3] = LoadGraph( "data/effect/lord_stick_e.bmp" );
	ghandle[4] = LoadGraph( "data/effect/lord_stick_f.bmp" );
	ghandle[5] = LoadGraph( "data/effect/lord_stick_g.bmp" );
	ghandle[6] = LoadGraph( "data/effect/lord_stick_h.bmp" );

	ghandle[7] = LoadGraph( "data/effect/lord_blast_stick_a.bmp" );
	ghandle[8] = LoadGraph( "data/effect/lord_blast_stick_b.bmp" );

	ghandle[9] = LoadGraph( "data/effect/lord_stick_plazma_a.bmp" );
	ghandle[10] = LoadGraph( "data/effect/lord_stick_plazma_b.bmp" );

	ghandle[11] = LoadGraph( "data/effect/blast_stick.bmp" );

	// test
	ghandle[12] = LoadGraph( "data/swordman001.bmp" );

	for( int i = 0; i < 13; i++ )
	{
		if( i == -1 )
			throw( -1 );
	}

	AttackAnim.setImage( 0, ghandle[0] );
	AttackAnim.setImage( 0, ghandle[1] );
	AttackAnim.setImage( 0, ghandle[2] );
	AttackAnim.setImage( 0, ghandle[3] );
	AttackAnim.setImage( 0, ghandle[4] );
	AttackAnim.setImage( 0, ghandle[5] );
	AttackAnim.setImage( 0, ghandle[6] );

	AttackAnim.setImage( 0, ghandle[7] );
	AttackAnim.setImage( 0, ghandle[8] );

	AttackAnim.setImage( 0, ghandle[9] );
	AttackAnim.setImage( 0, ghandle[10] );
	AttackAnim.setGap( 0, 6 );

	AttackAnim.setImage( 1, ghandle[11] );

	AttackAnim.setImage( 2, ghandle[12] );
	*/

	int ghandle[10];
	if( LoadDivGraph( "data/font/font_number.bmp", 10, 5, 2, 21, 23, ghandle ) == -1 ) throw( -1 );

	for( int i = 0; i < 10; i++ )
		EffectAnim.setImage( i, ghandle[i] );

	int ghandle2[1];
	if( LoadDivGraph( "data/cursor/cursor.bmp", 1, 1, 1, 82, 256, ghandle2 ) == -1 ) throw( -1 );

	EffectAnim.setImage( 10, ghandle2[0] );
}

/* ========================================================================= */
/* 関数名：CEffectControl::move												 */
/* 内容：エフェクトの処理													 */
/* 日付：2007/09/14															 */
/* ========================================================================= */
void CEffectControl::move( float JikiX, float JikiY )
{
	int x,y;
	GetMousePoint( &x, &y );
	// test
	EffectAnim.draw( 10, (float)x, (float)y );

	list<CharaData>::iterator it;
	it = effectlist.begin();

	while( it != effectlist.end() )
	{
		char data[64];
		sprintf_s( data, "%d", (*it).damage );
		drawDamage( (int)( (*it).x - JikiX + WIDTH/2 - 32/4 ), (int)( (*it).y - JikiY + HEIGHT/2 ), data, (*it).alpha );

		(*it).y -= 2;
		(*it).alpha -= 5;

		if( (*it).alpha < 5)
		{
			it = effectlist.erase( it );
		}
		else
		{
			++it;
		}
	}
}

/* ========================================================================= */
/* 関数名：CEffectControl::drawDamage										 */
/* 内容：ダメージの描画（半角なら文字の描画も応用可能）						 */
/* 日付：2007/09/14															 */
/* ========================================================================= */
void CEffectControl::drawDamage( int x, int y, char *str, int alpha )
{
	unsigned char alphabet;

//	int strx, stry;

//	int num;
	int i = 0;
	int size = strlen( str );

	while( *str != NULL )
	{
		alphabet = *str++;

		/*
		if( alphabet == 0x0a )
		{
			//改行
			break;
		}
		*/

		alphabet -= ' ';

		int num = alphabet % 0x10;

		//EffectAnim.draw( num, (float)(x + i * 10 - ( ( i*2+1 )*20/2 ) ), (float)y, alpha);
		EffectAnim.draw( num, (float)( x + (i * 10) - (size*7 + (size-1)*3)/2/* - (4*20/2)*/ ), (float)y, alpha);

		i++;
		/*
		strx = ( alphabet % 0x10 );
		stry = ( alphabet % 0x10 );
		*/

	}
}
