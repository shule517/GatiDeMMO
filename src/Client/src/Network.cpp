
#include "StdAfx.h"
#include "Network.h"

/* ========================================================================= */
/* 関数名：CNetwork::CNetwork												 */
/* 内容：コンストラクタ														 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
CNetwork::CNetwork( void )
{
	WSADATA wsaData;
	WSAStartup( MAKEWORD( 2, 0 ), &wsaData );
}

/* ========================================================================= */
/* 関数名：CNetwork::~CNetwork												 */
/* 内容：デストラクタ														 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
CNetwork::~CNetwork( void )
{
	closesocket( listen_socket );
	WSACleanup();
}

/* ========================================================================= */
/* 関数名：CNetwork::sendServer												 */
/* 内容：サーバーへ送信														 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
void CNetwork::sendServer( char *Buf )
{
	send( listen_socket, Buf, (int)strlen(Buf), 0 );
}

/* ========================================================================= */
/* 関数名：CNetwork::start													 */
/* 内容：ネットワークの開始													 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
void CNetwork::start( list<CharaData> *_MyList, list<CharaData> *_PlayerList, list<CharaData> *_EffectList )
{
	MyList = _MyList;
	PlayerList = _PlayerList;
	EffectList = _EffectList;

	// Connectスレッド生成
	_beginthread( ConnectThread, 0, this );
}

/* ========================================================================= */
/* 関数名：CNetwork::connect												 */
/* 内容：ネットワークの接続													 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
void CNetwork::connect()
{
	char	*ServerIp	= SERVER_IP;
	u_short	PortNo		= SERVER_PORT;

	SOCKADDR_IN		saddr;
	HOSTENT			*lpHost;
	unsigned int	addr;

	listen_socket = socket( AF_INET, SOCK_STREAM, 0 );
	lpHost = gethostbyname( ServerIp );
	if( lpHost == NULL )
	{
		addr	= inet_addr( ServerIp );
		lpHost	= gethostbyaddr( (char *)&addr, 4, AF_INET );
	}

	memset( &saddr, 0, sizeof( SOCKADDR_IN ) );
	saddr.sin_family		= lpHost->h_addrtype;
	saddr.sin_port			= htons( PortNo );
	saddr.sin_addr.s_addr	= *( (u_long *)lpHost->h_addr );

	if( ::connect( listen_socket, (SOCKADDR *)&saddr, sizeof( saddr ) ) == SOCKET_ERROR )
	{
		MB( "サーバーに接続できませんでした。\n再起動してください。" );
		// 終了
		_endthread();
	}

	// Clientスレッド生成
	_beginthread( ClientThread, 0, this );
}

/* ========================================================================= */
/* 関数名：ConnectThread													 */
/* 内容：Connectスレッド													 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
void ConnectThread( void *lpvoid )
{
	CNetwork *pNetwork = (CNetwork *)lpvoid;

	// 接続
	pNetwork->connect();

	// 終了
	_endthread();
}

/* ========================================================================= */
/* 関数名：ClientThread														 */
/* 内容：Clientメインスレッド												 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
void ClientThread( void *lpvoid )
{
	CNetwork *pNetwork = (CNetwork *)lpvoid;

	// クライアント
	pNetwork->doClientMain();

	_endthread();
}

/* ========================================================================= */
/* 関数名：CNetwork::doClientMain											 */
/* 内容：クライアントメイン処理												 */
/* 日付：2007/09/11															 */
/* ========================================================================= */
void CNetwork::doClientMain( void )
{
	WSAEVENT			hrevent;
	WSANETWORKEVENTS	event;
	char				message[1024];

	short	res = 0;
	int		len = 0;
	int		sizee = 0;
	char	data[MAX_PACKET_SIZE];

	// ユーザ認証
	send_PK_USER_AUTH( "id", "pw" );

	while( TRUE )
	{
		hrevent = WSACreateEvent();
		WSAEventSelect( listen_socket, hrevent, FD_WRITE | FD_READ | FD_CLOSE );

		while( TRUE )
		{
			WSAEnumNetworkEvents( listen_socket, hrevent, &event );

			if( event.lNetworkEvents & FD_READ )
			{
				memset( message, 0, sizeof( message ) );
				sizee = recv( listen_socket, message, sizeof( message ) - 1, 0 );

				len = 0;

				while( 1 )
				{
					// パケットの長さを取得
					memcpy( &res, &message[len], 2 );
					res -= 256;	// 調整

					if( res == sizee ) // パケットが1個
					{
						// パケット処理
						procMain( res, message );
						break;
					}
					else if( res < sizee && res > MIN_PACKET_SIZE )	// パケットが2個以上
					{
						memset( data, 0, MAX_PACKET_SIZE );
						memcpy( &data[0], &message[len], res);

						// パケット処理
						procMain( res, data );

						len += res;
					}
					else
					{
//						printf( "Error：パケットのサイズが一致しません。\n" );
						break;
					}

					if( len == sizee )
						break;
				}
			}
			else if( event.lNetworkEvents & FD_WRITE )
			{
				// 送信バッファに空きがある
			}
			else if( event.lNetworkEvents & FD_CLOSE )
			{
				MB( "サーバーキャンセル。\n再起動してください。" );
				// スレッドを終了
				_endthread();
			}
			Sleep( 1 );
		}
	}
}
