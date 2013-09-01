
#include "StdAfx.h"
#include "Network.h"

/* ========================================================================= */
/* �֐����FCNetwork::CNetwork												 */
/* ���e�F�R���X�g���N�^														 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
CNetwork::CNetwork( void )
{
	WSADATA wsaData;
	WSAStartup( MAKEWORD( 2, 0 ), &wsaData );
}

/* ========================================================================= */
/* �֐����FCNetwork::~CNetwork												 */
/* ���e�F�f�X�g���N�^														 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
CNetwork::~CNetwork( void )
{
	closesocket( listen_socket );
	WSACleanup();
}

/* ========================================================================= */
/* �֐����FCNetwork::sendServer												 */
/* ���e�F�T�[�o�[�֑��M														 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
void CNetwork::sendServer( char *Buf )
{
	send( listen_socket, Buf, (int)strlen(Buf), 0 );
}

/* ========================================================================= */
/* �֐����FCNetwork::start													 */
/* ���e�F�l�b�g���[�N�̊J�n													 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
void CNetwork::start( list<CharaData> *_MyList, list<CharaData> *_PlayerList, list<CharaData> *_EffectList )
{
	MyList = _MyList;
	PlayerList = _PlayerList;
	EffectList = _EffectList;

	// Connect�X���b�h����
	_beginthread( ConnectThread, 0, this );
}

/* ========================================================================= */
/* �֐����FCNetwork::connect												 */
/* ���e�F�l�b�g���[�N�̐ڑ�													 */
/* ���t�F2007/09/11															 */
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
		MB( "�T�[�o�[�ɐڑ��ł��܂���ł����B\n�ċN�����Ă��������B" );
		// �I��
		_endthread();
	}

	// Client�X���b�h����
	_beginthread( ClientThread, 0, this );
}

/* ========================================================================= */
/* �֐����FConnectThread													 */
/* ���e�FConnect�X���b�h													 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
void ConnectThread( void *lpvoid )
{
	CNetwork *pNetwork = (CNetwork *)lpvoid;

	// �ڑ�
	pNetwork->connect();

	// �I��
	_endthread();
}

/* ========================================================================= */
/* �֐����FClientThread														 */
/* ���e�FClient���C���X���b�h												 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
void ClientThread( void *lpvoid )
{
	CNetwork *pNetwork = (CNetwork *)lpvoid;

	// �N���C�A���g
	pNetwork->doClientMain();

	_endthread();
}

/* ========================================================================= */
/* �֐����FCNetwork::doClientMain											 */
/* ���e�F�N���C�A���g���C������												 */
/* ���t�F2007/09/11															 */
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

	// ���[�U�F��
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
					// �p�P�b�g�̒������擾
					memcpy( &res, &message[len], 2 );
					res -= 256;	// ����

					if( res == sizee ) // �p�P�b�g��1��
					{
						// �p�P�b�g����
						procMain( res, message );
						break;
					}
					else if( res < sizee && res > MIN_PACKET_SIZE )	// �p�P�b�g��2�ȏ�
					{
						memset( data, 0, MAX_PACKET_SIZE );
						memcpy( &data[0], &message[len], res);

						// �p�P�b�g����
						procMain( res, data );

						len += res;
					}
					else
					{
//						printf( "Error�F�p�P�b�g�̃T�C�Y����v���܂���B\n" );
						break;
					}

					if( len == sizee )
						break;
				}
			}
			else if( event.lNetworkEvents & FD_WRITE )
			{
				// ���M�o�b�t�@�ɋ󂫂�����
			}
			else if( event.lNetworkEvents & FD_CLOSE )
			{
				MB( "�T�[�o�[�L�����Z���B\n�ċN�����Ă��������B" );
				// �X���b�h���I��
				_endthread();
			}
			Sleep( 1 );
		}
	}
}
