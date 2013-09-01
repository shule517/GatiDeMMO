
#include "Network.h"

/* ========================================================================= */
/* �֐����FCNetwork::proc													 */
/* ���e�F��M�����p�P�b�g����												 */
/* ���t�F2007/08/28															 */
/* ========================================================================= */
void CNetwork::proc( int PlayerNo, int res, char *message )
{
	int		pLen = 2, header;
	WORD	nEnd, rEnd;
	nEnd = PEND;

	// �G���h�}�[�J�擾
	//memcpy( &rEnd, &message[res-2], 2 );

	//if( rEnd == nEnd )
	{
//		printf( "�G���h�}�[�J��v�I\n" );

		// �w�b�_�[���擾
		header = message[pLen];

		switch( header )
		{
			case PK_CMD_MV:
				proc_PK_CMD_MV( PlayerNo, res, message );
				break;

			case PK_USER_AUTH:
				proc_PK_USER_AUTH( PlayerNo, res, message );
				break;

			case PK_ATTACK:
				proc_PK_ATTACK( PlayerNo, res, message );
				break;

			default:
				printf( "����`�ȃp�P�b�g�����M����Ă��܂����B\n" );
				break;
		}
	}
//	else
	{
//		printf( "�G���h�}�[�J����v���Ȃ������̂ŏI���B\n" );
	}
}

/* ========================================================================= */
/* �֐����FCNetwork::procPacket												 */
/* ���e�F�p�P�b�g�̃f�[�^���o												 */
/* ���t�F2007/08/28															 */
/* ========================================================================= */
void CNetwork::procPacket( char *message, char *fmt, ... )
{
	va_list	ap;
	int		*d;
	char	*s;

	int len = 3;
	int charlen;

	va_start( ap, fmt );
	while( *fmt )
	{
		switch( *fmt++ )
		{
			case 's': // char
				// �f�[�^�擾
				s = va_arg( ap, char * );

				// �f�[�^�̒���
				charlen = message[len];
				len += 1;

				// �f�[�^
				memcpy( s, &message[len], charlen );
				s[charlen] = '\0';
				len += charlen;

//				printf( "%d|%s|", charlen, s );
				//printf( "char %s\n", s );
				break;

			case 'd': // int
				// �f�[�^�擾
				d = va_arg( ap, int * );

				// �f�[�^
				(*d) = message[len];
				len += 1;

//				printf( "%d|", (*d) );
				//printf( "int %d\n", (*d) );
				break;

			case 'h': // short
				// �f�[�^�擾
				d = va_arg( ap, int * );

				// �p�P�b�g�̒������擾
				memcpy( &(*d), &message[len], 2 );
				(*d) -= 256; // �f�[�^���킹

				len += 2;

//				printf( "%d|" , (*d) );
				//printf( "short %d\n", (*d) );
				break;
		}
	}
    va_end( ap );

//	printf( "PEND|\n" );
}

/* ========================================================================= */
/* �֐����FCNetwork::proc_PK_CMD_MV											 */
/* ���e�F�ړ��p�P�b�g��M													 */
/* ���t�F2007/09/04															 */
/* ========================================================================= */
void CNetwork::proc_PK_CMD_MV( int PlayerNo, int res, char *message )
{
/*	for( int i=0; i < res; i++ )
		printf( "%c", message[i] );
	printf( "\n" );

	printf( "|%d|%d|", res, message[2] );
*/
	int x = 0;
	int y = 0;
	int dx = 0;
	int dy = 0;

	procPacket( message, "%h %h %h %h", &x, &y, &dx, &dy );

//	printf( "x = %d\n", x);
//	printf( "y = %d\n", y);

	list<ClientData>::iterator it;
	it = ClientList.begin();

	while( it != ClientList.end() )
	{
		if( (*it).id == PlayerNo )
		{
			(*it).x = (float)x;
			(*it).y = (float)y;
			(*it).px = (*it).x;
			(*it).py = (*it).y;

			(*it).dx = (float)dx;
			(*it).dy = (float)dy;

			(*it).move_start_time = GetNowTime();

			float x = (*it).dx - (*it).x;
			float y = (*it).dy - (*it).y;
			float time = ( sqrtf( x*x + y*y ) / move_speed * (1.0f/60.0f) )*1000;
			(*it).move_end_time = GetNowTime() + (int)time;

//			(*it).state = STATE_MOVE;
			break;
		}
		++it;
	}

	// ����̃L�����Ɉړ���`����
	send_PK_OBJ_UPDATE_ACT( PlayerNo, dx, dy );
}

/* ========================================================================= */
/* �֐����FCNetwork::proc_PK_USER_AUTH										 */
/* ���e�F�F�؃p�P�b�g��M													 */
/* ���t�F2007/09/04															 */
/* ========================================================================= */
void CNetwork::proc_PK_USER_AUTH( int PlayerNo, int res, char *message )
{
	char id[256];
	char pw[256];

	procPacket( message, "%s %s", &id, &pw );

	// �F�؊����@�����̃L�����f�[�^�𑗐M
	send_PK_USER_AUTH( PlayerNo );

	// �v���C���[�L�����f�[�^�𑗐M
	sendNowPlayer( PlayerNo );
}

/* ========================================================================= */
/* �֐����FCNetwork::proc_PK_ATTACK											 */
/* ���e�F�U���p�P�b�g��M													 */
/* ���t�F2007/09/03															 */
/* ========================================================================= */
void CNetwork::proc_PK_ATTACK( int PlayerNo, int res, char *message )
{
	int x = 0;
	int y = 0;

	procPacket( message, "%h %h", &x, &y );

	list<ClientData>::iterator it;
	it = ClientList.begin();

	// scale * scale�̐����`�͈̔͂Ƀ_���[�W
	float scale = 50;

	while( it != ClientList.end() )
	{
		if( (float)( x-scale/2 ) < (*it).x && (*it).x < (float)( x+scale/2) )
		{
			if( (float)( y-scale/2 ) < (*it).y && (*it).y < (float)( y+scale/2 ) )
			{
				// �_���[�W
				send_PK_DAMAGE( PlayerNo, (*it).id, rand()%0xC8D1 );
			}
		}
		++it;
	}

	// ����̃L�����ɍU����`����

}
