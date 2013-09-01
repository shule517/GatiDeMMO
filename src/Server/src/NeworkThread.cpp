
#include "Network.h"

/* ========================================================================= */
/* 関数名：SynchroThread													 */
/* 内容：同期スレッド														 */
/* 日付：2007/09/14															 */
/* ========================================================================= */
void SynchroThread( void *lpvoid )
{
	CNetwork* pNetwork = (CNetwork *)lpvoid;

	// 同期開始
	pNetwork->synchro();

	_endthread ();
}

/* ========================================================================= */
/* 関数名：AcceptThread														 */
/* 内容：Acceptスレッド														 */
/* 日付：2007/08/23															 */
/* ========================================================================= */
void AcceptThread( void *lpvoid )
{
	CNetwork* pNetwork = (CNetwork *)lpvoid;

	SOCKADDR_IN	ClientAddr;
	SOCKET		tmp_socket;
	int			address_size;

	while( TRUE )
	{
		address_size = sizeof( ClientAddr );
		memset( &ClientAddr, 0, address_size );

		tmp_socket = accept( pNetwork->listen_socket, (struct sockaddr *)&ClientAddr, &address_size );

		if( tmp_socket == INVALID_SOCKET )
		{
			printf( "接続失敗\n" );
			continue;	// 失敗したらもう1度accept
		}
		else
		{
			printf( "%sが接続してきました。\n", inet_ntoa( ClientAddr.sin_addr ) );
		}

		pNetwork->add( tmp_socket, inet_ntoa( ClientAddr.sin_addr ) );

		// Clientスレッド生成
		_beginthread( ClientThread, 0, (void *)pNetwork );
	}
	_endthread ();
}

/* ========================================================================= */
/* 関数名：ClientThread														 */
/* 内容：Clientメインスレッド												 */
/* 日付：2007/08/23															 */
/* ========================================================================= */
void ClientThread( void *lpvoid )
{
	CNetwork			*pNetwork = (CNetwork *)lpvoid;
	WSAEVENT			hrevent;
	WSANETWORKEVENTS	event;
	char				message[1024];

	int PlayerNo;

	short	res = 0;
	int		len = 0;
	int		sizee = 0;
	char	data[MAX_PACKET_SIZE];

	ClientData *cd = pNetwork->searchUnused( &PlayerNo );
/**********************************************************************************/
	// 本当ならこの辺りで初期化っつーかログイン承認
//	printf( "PlayerNo:%d\n", PlayerNo );
	// 同期(現在接続している人たちと自分の情報を知らせる)
//	pNetwork->sendNowPlayer( PlayerNo );
/**********************************************************************************/
	while( TRUE )
	{
		hrevent = WSACreateEvent();
		WSAEventSelect( cd->socket, hrevent, FD_WRITE | FD_READ | FD_CLOSE );

		while( TRUE )
		{
			WSAEnumNetworkEvents( cd->socket, hrevent, &event );

			if( event.lNetworkEvents & FD_READ )
			{
				memset( message, 0, sizeof( message ) );
				sizee = recv( cd->socket, message, sizeof( message ) - 1, 0 );

				len = 0;

				if( sizee != -1 )
				{
					// パケット処理
					pNetwork->proc( PlayerNo, res, message );

					/*
					while( 1 )
					{
						
						// パケットの長さを取得
						memcpy( &res, &message[len], 2 );
						res -= 256;	// 調整

						if( res == sizee ) // パケットが1個
						{
							// パケット処理
							pNetwork->proc( PlayerNo, res, message );
							break;
						}
						else if( res < sizee && res > MIN_PACKET_SIZE )	// パケットが2個以上
						{
							memset( data, 0, MAX_PACKET_SIZE );
							memcpy( &data[0], &message[len], res);

							// パケット処理
							pNetwork->proc( PlayerNo, res, data );

							len += res;
						}
						else
						{
							printf( "Error：パケットのサイズが異常です。\n" );
							break;
						}

						if( len == sizee )
							break;
					}
					*/
				}
			}
			else if( event.lNetworkEvents & FD_WRITE )
			{
				// 送信バッファに空きがある
				//printf( "FD_WRITE\n" );
			}
			else if( event.lNetworkEvents & FD_CLOSE )
			{
				pNetwork->erase( PlayerNo );				// サーバのキャラリストから消去
				pNetwork->send_PK_OBJ_REMOVE( PlayerNo );	// キャラ削除パケットを送信
				printf( "%dのスレッド開放します。\n", PlayerNo );
				_endthread();
			}
			Sleep( 1 );
		}
	}
	_endthread();
}
