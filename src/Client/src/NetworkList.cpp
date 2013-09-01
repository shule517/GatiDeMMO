
#include "StdAfx.h"
#include "Network.h"

/* ========================================================================= */
/* 関数名：CNetwork::addPlayerList											 */
/* 内容：マイリストにキャラクターを追加										 */
/* 日付：2007/09/12															 */
/* ========================================================================= */
void CNetwork::addMyList( int PlayerNo, char *Name, int x, int y )
{
	// とりあえずデータの訂正だけ
	list<CharaData>::iterator it;
	it = MyList->begin();

	(*it).id = PlayerNo;
	sprintf_s( (*it).name, "%s", Name );
	(*it).x = (float)x;
	(*it).y = (float)y;
	(*it).dx = (*it).x;
	(*it).dy = (*it).y;
	(*it).dir = 0;
	(*it).hp = 600;
	(*it).hp_max = 1000;

	/*
	// 同じIDのキャラがいないか確認
	list<CharaData>::iterator it;
	it = MyList->begin();

	while( it != MyList->end() )
	{
		if( (*it).id == PlayerNo )
		{
			// 関数脱出
			return;
		}
		++it;
	}

	// キャラ追加
	CharaData temp;
	temp.id = PlayerNo;
	sprintf_s( temp.name, "%s", Name );
	temp.x = (float)x;
	temp.y = (float)y;
	temp.dx = temp.x;
	temp.dy = temp.y;
	temp.dir = 0;
	// temp.anim = &CharaAnim;

	MyList->push_back( temp );	// キャラデータセット
	*/
}

/* ========================================================================= */
/* 関数名：CNetwork::addPlayerList											 */
/* 内容：プレイヤーリストにキャラクターを追加								 */
/* 日付：2007/09/12															 */
/* ========================================================================= */
void CNetwork::addPlayerList( int PlayerNo, char *Name, int x, int y )
{
	// 自分のキャラに同じIDがいないか確認
	list<CharaData>::iterator it;
	it = MyList->begin();

	while( it != MyList->end() )
	{
		if( (*it).id == PlayerNo )
		{
			// 関数脱出
			return;
		}
		++it;
	}

	// 同じIDのキャラがいないか確認
//	list<CharaData>::iterator it;
	it = PlayerList->begin();

	while( it != PlayerList->end() )
	{
		if( (*it).id == PlayerNo )
		{
			// 関数脱出
			return;
		}
		++it;
	}

	// キャラ追加
	CharaData temp;
	temp.id = PlayerNo;
	sprintf_s( temp.name, "%s", Name );
	temp.x = (float)x;
	temp.y = (float)y;
	temp.dx = temp.x;
	temp.dy = temp.y;
	temp.dir = 0;
	temp.hp = 1000;
	temp.hp_max = 1000;
	// temp.anim = &CharaAnim;

	PlayerList->push_back( temp );	// キャラデータセット
}

/* ========================================================================= */
/* 関数名：CNetwork::addEffectList											 */
/* 内容：エフェクトをリストに追加											 */
/* 日付：2007/09/14															 */
/* ========================================================================= */
void CNetwork::addEffectList( int x, int y, int damage )
{
	list<CharaData>::iterator it;
	it = EffectList->begin();

	CharaData effect;
	effect.x = (float)x;
	effect.y = (float)y;
	effect.damage = damage;
	effect.alpha = 255;

	EffectList->push_back( effect );
}

/* ========================================================================= */
/* 関数名：CNetwork::movePlayer												 */
/* 内容：キャラクターの移動													 */
/* 日付：2007/09/12															 */
/* ========================================================================= */
void CNetwork::movePlayer( int PlayerNo, int dx, int dy )
{
	list<CharaData>::iterator it;
	it = PlayerList->begin();

	while( it != PlayerList->end() )
	{
		if( (*it).id == PlayerNo )
		{
			(*it).px = (*it).x;
			(*it).py = (*it).y;

			(*it).dx = (float)dx;
			(*it).dy = (float)dy;

			(*it).move_start_time = GetNowTime();

			float x = (*it).dx - (*it).x;
			float y = (*it).dy - (*it).y;
			float time = ( sqrtf( x*x + y*y ) / move_speed * (1.0f/60.0f) )*1000;
			(*it).move_end_time = GetNowTime() + (int)time;

			(*it).state = STATE_MOVE;
			break;
		}
		++it;
	}
}

/* ========================================================================= */
/* 関数名：CNetwork::erasePlayer											 */
/* 内容：キャラクターの削除													 */
/* 日付：2007/09/12															 */
/* ========================================================================= */
void CNetwork::erasePlayer( int PlayerNo )
{
	list<CharaData>::iterator it;
	it = PlayerList->begin();

	while( it != PlayerList->end() )
	{
		if( (*it).id == PlayerNo )
		{
			(*it).id = -1;
			break;
		}
		else
		{
			++it;
		}
	}
/*
	list<CharaData>::iterator it;
	it = PlayerList->begin();

	int size = PlayerList->size();

	// エラー対策キャラと消したいキャラで2キャラ以上いるなら削除可能
	if( size >= 2 )
	{
		while( it != PlayerList->end() )
		{
			if( (*it).id == PlayerNo )
			{
				PlayerList->erase( it );
				return;
			}
			++it;
		}
	}
*/
}

/* ========================================================================= */
/* 関数名：CNetwork::damagePlayer											 */
/* 内容：ダメージ処理														 */
/* 日付：2007/09/14															 */
/* ========================================================================= */
void CNetwork::damagePlayer( int PlayerNo, int damage )
{
	// ダメージを受けたのが自分
	list<CharaData>::iterator it;
	it = MyList->begin();

	while( it != MyList->end() )
	{
		if( (*it).id == PlayerNo )
		{
			(*it).hp -= damage;
			if( (*it).hp < 0)
				(*it).hp = 0;
			// エフェクト追加
			addEffectList( (int)(*it).x, (int)(*it).y, damage );
			// 関数脱出
			return;
		}
		++it;
	}

	// ダメージを受けたのがプレイヤー
	it = PlayerList->begin();

	while( it != PlayerList->end() )
	{
		if( (*it).id == PlayerNo )
		{
			(*it).hp -= damage;
			if( (*it).hp < 0)
				(*it).hp = 0;
			// エフェクト追加
			addEffectList( (int)(*it).x, (int)(*it).y, damage );
			// 関数脱出
			return;
		}
		++it;
	}
}
