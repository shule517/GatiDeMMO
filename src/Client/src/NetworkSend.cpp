
#include "StdAfx.h"
#include "Network.h"

/* ========================================================================= */
/* �֐����FCNetwork::sendPacket												 */
/* ���e�F�p�P�b�g������Ă��瑗�M											 */
/* ���t�F2007/09/03															 */
/* ========================================================================= */
void CNetwork::sendPacket( char *fmt, ... )
{
	va_list	ap;
	int		d;
	char	*s;

	char			message[MAXLINE];
	unsigned short	nEnd = PEND;
	int				len, charlen;

	memset( &message, 0, MAXLINE );	// ������
	len = 2;
 
//	printf( "***********�p�P�b�g�쐬***********\n" );

	va_start( ap, fmt );
	while( *fmt )
	{
		switch( *fmt++ )
		{
		case 's':
			// �f�[�^�擾
			s = va_arg( ap, char * );

			// �f�[�^�̒���
			charlen = (int)strlen( s );
			message[len] = charlen;
			len += 1;

			// �f�[�^
			memcpy( &message[len], s, charlen );
			len += charlen;

//			printf( "�����F%d+1 char�F%s\n", charlen, s );
			break;

		case 'd':
			// �f�[�^�擾
			d = va_arg( ap, int );

			// �f�[�^
			message[len] = d;
			len += 1;
//			printf( "�����F1 int�F%d\n", d );
			break;

		case 'h':
			// �f�[�^�擾
			d = va_arg( ap, int );

			d += 256;	// �f�[�^���킹
			memcpy( &message[len], &d, 2 );

			if( d < 256 )
				MB( "%h Error�FCreatePacket\n" );

			len += 2;
//			printf( "�����F2 short�F%d\n", d-256 );
			break;
		}
	}
    va_end( ap );

	// �G���h�}�[�J
	memcpy( &message[len], &nEnd, 2 );
	len += 2;

	// �f�[�^��킹�̂��� �� �ォ��256�������Ȃ��Ƃ����Ȃ�
	len += 256;

	// �p�P�b�g�̒���
	memcpy( &message[0], &len, 2 );

	short res;
	memcpy( &res, &message[0], 2 );

	// �p�P�b�g�����`�F�b�N
	for( int i = 0; i < len-256; i++ )
	{
		if( message[i] == 0 )
		{
			//MB( "Error�F�p�P�b�g�����iCreatePacket�j" );
		}
	}

//	printf( "�p�P�b�g�S�̂̒����F%d(2+2)\n", res-256 );

	/*
	// �Ƃ肠�����\�����Ă݂�
	for( int i=0; i < len - 256; i++ )
		printf( "%c", message[i] );
	printf( "\n**********************************\n" );
	*/

	sendServer( message );
}

/* ========================================================================= */
/* �֐����FCNetwork::send_PK_CMD_MV											 */
/* ���e�F�ړ��p�P�b�g���M													 */
/* ���t�F2007/09/03															 */
/* ========================================================================= */
void CNetwork::send_PK_CMD_MV( int x, int y, int dx, int dy )
{
	sendPacket( "%d %h %h %h %h", PK_CMD_MV, x, y, /*x*/dx, /*y*/dy );
}

/* ========================================================================= */
/* �֐����FCNetwork::send_PK_CMD_MV											 */
/* ���e�F�F�؃p�P�b�g���M													 */
/* ���t�F2007/09/03															 */
/* ========================================================================= */
void CNetwork::send_PK_USER_AUTH( char *id, char *pw )
{
	sendPacket( "%d %s %s", PK_USER_AUTH, id, pw );
}

/* ========================================================================= */
/* �֐����FCNetwork::send_PK_ATTACK											 */
/* ���e�F�U���p�P�b�g���M													 */
/* ���t�F2007/09/14															 */
/* ========================================================================= */
void CNetwork::send_PK_ATTACK( int x, int y )
{
	sendPacket( "%d %h %h", PK_ATTACK, x, y );
}
