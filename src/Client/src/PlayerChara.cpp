
#include "StdAfx.h"
#include "PlayerChara.h"

/* ========================================================================= */
/* 関数名：CPlayerChara::CPlayerChara										 */
/* 内容：コンストラクタ														 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
CPlayerChara::CPlayerChara( void )
{
}

/* ========================================================================= */
/* 関数名：CPlayerChara::~CPlayerChara										 */
/* 内容：デストラクタ														 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
CPlayerChara::~CPlayerChara( void )
{
}

/* ========================================================================= */
/* 関数名：CPlayerChara::init												 */
/* 内容：初期化																 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
void CPlayerChara::init( void )
{
	PlayerList.clear();
}

/* ========================================================================= */
/* 関数名：CPlayerChara::loadFiles											 */
/* 内容：ファイルの読み込み													 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
void CPlayerChara::loadFiles() throw( int )
{
	int ghandle[16];
	if( LoadDivGraph( "data/swordman001.bmp", 16, 4, 4, 32, 32, ghandle ) == -1 ) throw( -1 );

	PlayerAnim.setImage( 0, ghandle[0] );
	PlayerAnim.setImage( 0, ghandle[1] );
	PlayerAnim.setImage( 0, ghandle[2] );
	PlayerAnim.setImage( 0, ghandle[3] );
	PlayerAnim.setGap( 0, 6 );

	PlayerAnim.setImage( 1, ghandle[4] );
	PlayerAnim.setImage( 1, ghandle[5] );
	PlayerAnim.setImage( 1, ghandle[6] );
	PlayerAnim.setImage( 1, ghandle[7] );
	PlayerAnim.setGap( 1, 6 );

	PlayerAnim.setImage( 2, ghandle[8] );
	PlayerAnim.setImage( 2, ghandle[9] );
	PlayerAnim.setImage( 2, ghandle[10] );
	PlayerAnim.setImage( 2, ghandle[11] );
	PlayerAnim.setGap( 2, 6 );

	PlayerAnim.setImage( 3, ghandle[12] );
	PlayerAnim.setImage( 3, ghandle[13] );
	PlayerAnim.setImage( 3, ghandle[14] );
	PlayerAnim.setImage( 3, ghandle[15] );
	PlayerAnim.setGap( 3, 6 );
}

/* ========================================================================= */
/* 関数名：CPlayerChara::move												 */
/* 内容：プレイヤーキャラクターの処理										 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
void CPlayerChara::move( float JikiX, float JikiY )
{
	move2();

	list<CharaData>::iterator it;
	it = PlayerList.begin();

	while( it != PlayerList.end() )
	{
		// キャラクターが削除されてないか確認
		if( (*it).id > 0 )
		{
			/*
			// キャラクター移動
			if( (*it).x < (*it).dx )
			{	// 右へ移動
				(*it).x += CGameFrame::IdouHosei( 4 );
				if( (*it).x > (*it).dx )
					(*it).x = (*it).dx;

				(*it).dir = 2;
			}
			if( (*it).x > (*it).dx )
			{	// 左へ移動
				(*it).x -= CGameFrame::IdouHosei( 4 );
				if( (*it).x < (*it).dx )
					(*it).x = (*it).dx;

				(*it).dir = 1;
			}
			if( (*it).y < (*it).dy )
			{	// 下へ移動
				(*it).y += CGameFrame::IdouHosei( 4 );
				if( (*it).y > (*it).dy )
					(*it).y = (*it).dy;

				(*it).dir = 0;
			}
			if( (*it).y > (*it).dy )
			{	// 上へ移動
				(*it).y -= CGameFrame::IdouHosei( 4 );
				if( (*it).y < (*it).dy )
					(*it).y = (*it).dy;

				(*it).dir = 3;
			}
			*/

			PlayerAnim.draw( (*it).dir, (*it).x - JikiX + WIDTH/2 - 32/2, (*it).y - JikiY + HEIGHT/2 - 32/2 );
	//		(*it).anim->draw( 0, (*it).x, (*it).y );

			char data[256];
			sprintf_s( data, "%d：%s", (*it).id, (*it).name );
			int x = (int)((*it).x - JikiX + WIDTH/2);
			int y = (int)((*it).y - JikiY + HEIGHT/2);
			DrawString( x - strlen( data )*5/2, y + 30, data, RGB( 255, 0, 0 ) );
			/*
			sprintf_s( data, "(%f,%f)", (*it).x, (*it).y );
			DrawString( 10, 10, data, RGB( 255, 0, 0 ) );

			sprintf_s( data, "(%f,%f)", (*it).dx, (*it).dy );
			DrawString( 10, 30, data, RGB( 255, 0, 0 ) );
			*/
		}

		++it;
	}

	/*
	// プレイヤー削除
	if( (*it).id < 1 )
	{
		it = PlayerList.erase( it );
	}
	else
	{
		++it;
	}

	*/
	it = PlayerList.begin();

	while( it != PlayerList.end() )
	{
		if( (*it).id < 1 )
		{
			PlayerList.erase( it );
			break;
		}
		++it;
	}
}

void CPlayerChara::move2()
{
	list<CharaData>::iterator it;
	it = PlayerList.begin();

	while( it != PlayerList.end() )
	{
		if( (*it).state == STATE_MOVE )
		{
			float DiffTime = ( GetNowTime() - (*it).move_start_time ) / 1000.0f;

			if( GetNowTime() >= (*it).move_end_time )
			{
				(*it).x = (*it).dx;
				(*it).y = (*it).dy;
				(*it).state = STATE_STOP;
				break;
			}

			float time = (float)( (float)( (*it).move_end_time - (*it).move_start_time ) / 1000.0f );
			DiffTime /= time;

			(*it).x = (float)( ( (float)(*it).dx - (float)(*it).px ) * (float)DiffTime + (float)(*it).px );
			(*it).y = (float)( ( (float)(*it).dy - (float)(*it).py ) * (float)DiffTime + (float)(*it).py );
		}
		++it;
	}
}
