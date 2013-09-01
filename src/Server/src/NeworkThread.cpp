
#include "Network.h"

/* ========================================================================= */
/* �֐����FSynchroThread													 */
/* ���e�F�����X���b�h														 */
/* ���t�F2007/09/14															 */
/* ========================================================================= */
void SynchroThread( void *lpvoid )
{
	CNetwork* pNetwork = (CNetwork *)lpvoid;

	// �����J�n
	pNetwork->synchro();

	_endthread ();
}

/* ========================================================================= */
/* �֐����FAcceptThread														 */
/* ���e�FAccept�X���b�h														 */
/* ���t�F2007/08/23															 */
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
			printf( "�ڑ����s\n" );
			continue;	// ���s���������1�xaccept
		}
		else
		{
			printf( "%s���ڑ����Ă��܂����B\n", inet_ntoa( ClientAddr.sin_addr ) );
		}

		pNetwork->add( tmp_socket, inet_ntoa( ClientAddr.sin_addr ) );

		// Client�X���b�h����
		_beginthread( ClientThread, 0, (void *)pNetwork );
	}
	_endthread ();
}

/* ========================================================================= */
/* �֐����FClientThread														 */
/* ���e�FClient���C���X���b�h												 */
/* ���t�F2007/08/23															 */
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
	// �{���Ȃ炱�̕ӂ�ŏ��������[�����O�C�����F
//	printf( "PlayerNo:%d\n", PlayerNo );
	// ����(���ݐڑ����Ă���l�����Ǝ����̏���m�点��)
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
					// �p�P�b�g����
					pNetwork->proc( PlayerNo, res, message );

					/*
					while( 1 )
					{
						
						// �p�P�b�g�̒������擾
						memcpy( &res, &message[len], 2 );
						res -= 256;	// ����

						if( res == sizee ) // �p�P�b�g��1��
						{
							// �p�P�b�g����
							pNetwork->proc( PlayerNo, res, message );
							break;
						}
						else if( res < sizee && res > MIN_PACKET_SIZE )	// �p�P�b�g��2�ȏ�
						{
							memset( data, 0, MAX_PACKET_SIZE );
							memcpy( &data[0], &message[len], res);

							// �p�P�b�g����
							pNetwork->proc( PlayerNo, res, data );

							len += res;
						}
						else
						{
							printf( "Error�F�p�P�b�g�̃T�C�Y���ُ�ł��B\n" );
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
				// ���M�o�b�t�@�ɋ󂫂�����
				//printf( "FD_WRITE\n" );
			}
			else if( event.lNetworkEvents & FD_CLOSE )
			{
				pNetwork->erase( PlayerNo );				// �T�[�o�̃L�������X�g�������
				pNetwork->send_PK_OBJ_REMOVE( PlayerNo );	// �L�����폜�p�P�b�g�𑗐M
				printf( "%d�̃X���b�h�J�����܂��B\n", PlayerNo );
				_endthread();
			}
			Sleep( 1 );
		}
	}
	_endthread();
}
