
#include "Network.h"

/* ========================================================================= */
/* �֐����FCNetwork::init													 */
/* ���e�F������																 */
/* ���t�F2007/08/23															 */
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

	// Accept�X���b�h����
	_beginthread( AcceptThread, 0, (void *)this );

	Sleep( 500 );

	// �����X���b�h����
	_beginthread( SynchroThread, 0, (void *)this );
}

/* ========================================================================= */
/* �֐����FCNetwork::add													 */
/* ���e�F�f�[�^�̒ǉ�														 */
/* ���t�F2007/08/23															 */
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
/* �֐����FCNetwork::get													 */
/* ���e�F�f�[�^�̎擾														 */
/* ���t�F2007/08/23															 */
/* ========================================================================= */
ClientData *CNetwork::get( int no )
{
	list<ClientData>::iterator it;
	it = ClientList.begin();

	if( no < (int)ClientList.size() )
	{
		// �w�肵���v�f�����鎞
		for( int i=0; i < no; i++)
			++it;

		return &(*it);
	}
	else
	{
		// �w�肵���v�f���Ȃ���
		ClientData cd;
		return &cd;
	}
}

/* ========================================================================= */
/* �֐����FCNetwork::searchUnused											 */
/* ���e�F�܂��g���Ă��Ȃ�ClientData��T��									 */
/* ���t�F2007/08/23															 */
/* ========================================================================= */
ClientData *CNetwork::searchUnused( int *PlayerNo )
{
	list<ClientData>::iterator it;
	it = ClientList.begin();

	while( it == ClientList.end() || (*it).id != -1)
		++it;

	srand( GetTickCount() ); //�����g�p
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
/* �֐����FCNetwork::erase													 */
/* ���e�F���X�g�̃f�[�^���w�肵�č폜										 */
/* ���t�F2007/08/23															 */
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
/* �֐����FCNetwork::sendPacket												 */
/* ���e�F�p�P�b�g�𑗐M														 */
/* ���t�F2007/08/23															 */
/* ========================================================================= */
void CNetwork::sendPacket( SENDMODE mode, int PlayerNo, char *Buf )
{
	list<ClientData>::iterator it;
	it = ClientList.begin();

	switch( mode )
	{
	case SEND_ME:	// ���������ɑ���
		while( it != ClientList.end() )
		{
			if( (*it).id == PlayerNo )
			{
				// �����ɑ�������֐��E�o
				send( (*it).socket, Buf, (int)strlen(Buf), 0 );
				return;
			}
			++it;
		}
		break;

	case SEND_ALL:	// �����ȊO�S���ɑ���
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
/* �֐����FCNetwork::synchro												 */
/* ���e�F����																 */
/* ���t�F2007/09/14															 */
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
			// �L�����N�^�[�ړ�
			if( (*it).x < (*it).dx )
			{
				(*it).x += move_speed;
				if( (*it).x > (*it).dx )
				{
					(*it).x = (*it).dx;
					printf( "�ړ�����\n" );
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
			// �L�����N�^�[�ړ�
			if( (*it).x < (*it).dx )
			{	// �E�ֈړ�
				unsigned int a = GetNowTime() - (*it).move_start;
				unsigned int b = (*it).move_time_x - (*it).move_start;
				float c = (*it).dx - (*it).x;

				(*it).x += (float)a/(float)b*c;

				if( (*it).x > (*it).dx )
					(*it).x = (*it).dx;
			}
			if( (*it).x > (*it).dx )
			{	// ���ֈړ�
				unsigned int a = GetNowTime() - (*it).move_start;
				unsigned int b = (*it).move_time_x - (*it).move_start;
				float c = (*it).dx - (*it).x;

				(*it).x += (float)a/(float)b*c;

				if( (*it).x < (*it).dx )
					(*it).x = (*it).dx;
			}
			if( (*it).y < (*it).dy )
			{	// ���ֈړ�
				unsigned int a = GetNowTime() - (*it).move_start;
				unsigned int b = (*it).move_time_y - (*it).move_start;
				float c = (*it).dy - (*it).y;

				(*it).y += (float)a/(float)b*c;

				if( (*it).y > (*it).dy )
					(*it).y = (*it).dy;
			}
			if( (*it).y > (*it).dy )
			{	// ��ֈړ�
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
