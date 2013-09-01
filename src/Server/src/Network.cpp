
#include "Network.h"

/* ========================================================================= */
/* 関数名：CNetwork::init													 */
/* 内容：初期化																 */
/* 日付：2007/08/23															 */
/* ========================================================================= */
void CNetwork::init( u_short PortNo )
{
	WSADATA		wsaData;
	SOCKADDR_IN	ServerAddr;

	WSAStartup( MAKEWORD( 2, 2 ) , &wsaData );

	listen_socket = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED );

	ServerAddr.sin_family		= AF_INET;
	ServerAddr.sin_port			= htons( PortNo );
	ServerAddr.sin_addr.s_addr	= htonl( INADDR_ANY );

	bind( listen_socket, (SOCKADDR *)&ServerAddr, sizeof( SOCKADDR_IN ) );
	listen( listen_socket, 5 );

	// Acceptスレッド生成
	_beginthread( AcceptThread, 0, (void *)this );

	Sleep( 500 );

	// 同期スレッド生成
	_beginthread( SynchroThread, 0, (void *)this );
}

/* ========================================================================= */
/* 関数名：CNetwork::add													 */
/* 内容：データの追加														 */
/* 日付：2007/08/23															 */
/* ========================================================================= */
void CNetwork::add( SOCKET socket, char *ip )
{
	ClientData cd;

	cd.socket = socket;
	cd.ip = ip;
	cd.x = 1;//rand()%550;
	cd.y = 1;//rand()%350;

	ClientList.push_back( cd );
}

/* ========================================================================= */
/* 関数名：CNetwork::get													 */
/* 内容：データの取得														 */
/* 日付：2007/08/23															 */
/* ========================================================================= */
ClientData *CNetwork::get( int no )
{
	list<ClientData>::iterator it;
	it = ClientList.begin();

	if( no < (int)ClientList.size() )
	{
		// 指定した要素がある時
		for( int i=0; i < no; i++)
			++it;

		return &(*it);
	}
	else
	{
		// 指定した要素がない時
		ClientData cd;
		return &cd;
	}
}

/* ========================================================================= */
/* 関数名：CNetwork::searchUnused											 */
/* 内容：まだ使われていないClientDataを探す									 */
/* 日付：2007/08/23															 */
/* ========================================================================= */
ClientData *CNetwork::searchUnused( int *PlayerNo )
{
	list<ClientData>::iterator it;
	it = ClientList.begin();

	while( it == ClientList.end() || (*it).id != -1)
		++it;

	srand( GetTickCount() ); //乱数使用
	(*PlayerNo) = rand();

	list<ClientData>::iterator it2;
	it2 = ClientList.begin();

	while( it2 != ClientList.end() )
	{
		if( (*it2).id == (*PlayerNo) )
		{
			(*PlayerNo) = rand();
			continue;
		}
		++it2;
	}

	(*it).id = (*PlayerNo);

	return &(*it);
}

/* ========================================================================= */
/* 関数名：CNetwork::erase													 */
/* 内容：リストのデータを指定して削除										 */
/* 日付：2007/08/23															 */
/* ========================================================================= */
void CNetwork::erase( int PlayerNo )
{
	list<ClientData>::iterator it;
	it = ClientList.begin();

	while( it != ClientList.end() )
	{
		if( (*it).id == PlayerNo )
		{
			it = ClientList.erase( it );
			break;
		}
		else
		{
			++it;
		}
	}
}

/* ========================================================================= */
/* 関数名：CNetwork::sendPacket												 */
/* 内容：パケットを送信														 */
/* 日付：2007/08/23															 */
/* ========================================================================= */
void CNetwork::sendPacket( SENDMODE mode, int PlayerNo, char *Buf )
{
	list<ClientData>::iterator it;
	it = ClientList.begin();

	switch( mode )
	{
	case SEND_ME:	// 自分だけに送る
		while( it != ClientList.end() )
		{
			if( (*it).id == PlayerNo )
			{
				// 自分に送ったら関数脱出
				send( (*it).socket, Buf, (int)strlen(Buf), 0 );
				return;
			}
			++it;
		}
		break;

	case SEND_ALL:	// 自分以外全員に送る
		while( it != ClientList.end() )
		{
			if( (*it).id != PlayerNo )
			{
				send( (*it).socket, Buf, (int)strlen(Buf), 0 );
			}
			++it;
		}
		break;
	}
}

/* ========================================================================= */
/* 関数名：CNetwork::synchro												 */
/* 内容：同期																 */
/* 日付：2007/09/14															 */
/* ========================================================================= */
void CNetwork::synchro()
{
	float move_speed = 1.873f;
	list<ClientData>::iterator it;

	while( 1 )
	{
		it = ClientList.begin();

		while( it != ClientList.end() )
		{
			/*
			// キャラクター移動
			if( (*it).x < (*it).dx )
			{
				(*it).x += move_speed;
				if( (*it).x > (*it).dx )
				{
					(*it).x = (*it).dx;
					printf( "移動完了\n" );
				}
			}
			if( (*it).x > (*it).dx )
			{
				(*it).x -= move_speed;
				if( (*it).x < (*it).dx )
				{
					(*it).x = (*it).dx;
				}
			}
			if( (*it).y < (*it).dy )
			{
				(*it).y += move_speed;
				if( (*it).y > (*it).dy )
				{
					(*it).y = (*it).dy;
				}
			}
			if( (*it).y > (*it).dy )
			{
				(*it).y -= move_speed;
				if( (*it).y < (*it).dy )
				{
					(*it).y = (*it).dy;
				}
			}
			*/

			/*
			// キャラクター移動
			if( (*it).x < (*it).dx )
			{	// 右へ移動
				unsigned int a = GetNowTime() - (*it).move_start;
				unsigned int b = (*it).move_time_x - (*it).move_start;
				float c = (*it).dx - (*it).x;

				(*it).x += (float)a/(float)b*c;

				if( (*it).x > (*it).dx )
					(*it).x = (*it).dx;
			}
			if( (*it).x > (*it).dx )
			{	// 左へ移動
				unsigned int a = GetNowTime() - (*it).move_start;
				unsigned int b = (*it).move_time_x - (*it).move_start;
				float c = (*it).dx - (*it).x;

				(*it).x += (float)a/(float)b*c;

				if( (*it).x < (*it).dx )
					(*it).x = (*it).dx;
			}
			if( (*it).y < (*it).dy )
			{	// 下へ移動
				unsigned int a = GetNowTime() - (*it).move_start;
				unsigned int b = (*it).move_time_y - (*it).move_start;
				float c = (*it).dy - (*it).y;

				(*it).y += (float)a/(float)b*c;

				if( (*it).y > (*it).dy )
					(*it).y = (*it).dy;
			}
			if( (*it).y > (*it).dy )
			{	// 上へ移動
				unsigned int a = GetNowTime() - (*it).move_start;
				unsigned int b = (*it).move_time_y - (*it).move_start;
				float c = (*it).dy - (*it).y;

				(*it).y += (float)a/(float)b*c;

				if( (*it).y < (*it).dy )
					(*it).y = (*it).dy;
			}
			*/
			moveChara();
			++it;
		}
		Sleep( 1 );
	}
}

void CNetwork::moveChara()
{
	list<ClientData>::iterator it;
	it = ClientList.begin();

	while( it != ClientList.end() )
	{
		float DiffTime = ( GetNowTime() - (*it).move_start_time ) / 1000.0f;

		if( GetNowTime() >= (*it).move_end_time )
		{
			(*it).x = (*it).dx;
			(*it).y = (*it).dy;
//			(*it).state = STATE_STOP;
			return;
		}

		float time = (float)( (float)( (*it).move_end_time - (*it).move_start_time ) / 1000.0f );
		DiffTime /= time;

		(*it).x = (float)( ( (float)(*it).dx - (float)(*it).px ) * (float)DiffTime + (float)(*it).px );
		(*it).y = (float)( ( (float)(*it).dy - (float)(*it).py ) * (float)DiffTime + (float)(*it).py );

		++it;
	}
}
